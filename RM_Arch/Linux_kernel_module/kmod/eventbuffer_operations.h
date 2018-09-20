/**
 * @file eventbuffer_operations.h
 *
 * This file contains declarations for manipulating event buffers such as event pushing and reading, index readers,
 * last overwritten timestamp readers, etc. Event Buffer manipulation should be done with these functions only.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */

#ifndef KERNEL_ARRAY_OPERATIONS_H
#define KERNEL_ARRAY_OPERATIONS_H

#include <asm/barrier.h>
#include <linux/compiler.h> /* WRITE_ONCE & READ_ONCE*/
#include <linux/uaccess.h>
#include <linux/string.h>

#include "raven.h"

/**
 * Reads the last overwritten timestamp from a buffer.
 *
 * If the current architecture is x64 then ktime_t, which has event_buffers_length of 8 bytes can fit inside a register and the read is atomic. Still one must be careful to not let the compiler  fold, spindle, or otherwise mutilate accesses.
 *  Otherwise a seqlock must be used when reading the last_overwritten timestamp to avoid race conditions.
 *
 * @param buffer The buffer to read the last overwritten from
 * @param last_overwritten a pointer to a ktime_t where the buffer last overwritten will be saved to
 */
void readLastOverwritten(struct raven_event_buffer *buffer, ktime_t *last_overwritten) {
#if BITS_PER_LONG >= 64
        *last_overwritten = READ_ONCE(buffer->last_overwritten);
#else
    unsigned int seq;
    do {
        seq = read_seqbegin(&buffer->last_overwritten_lock);
        *last_overwritten = buffer->last_overwritten;
    } while (unlikely(read_seqretry(&buffer->last_overwritten_lock, seq)));
#endif
}


/**
 * Reads a time index from a buffer.
 *
 * Reads a time index from a buffer ensuring the read is atomic. If the current architecture is x64 then ktime_t, which
 * has event_buffers_length of 8 bytes can fit inside a register and the read is atomic. Still one must be careful to not let the
 * compiler fold, spindle, or otherwise mutilate accesses.
 *
 * Otherwise the read must be synchronized using a seqlock to ensure it is correct.
 *
 * For information on how this is synchronized using a custom sequential lock, please see the
 * documentation on the struct raven_buffer_lock structure.
 *
 * @param buffer a buffer to read the time from
 * @param reader_index the index to read the buffer from
 * @param event_time a pointer to a ktime_t where the time read from the buffer will be saved to
 *
 * @warning the index must be valid otherwise an overflow will occur
 *
 * @see struct raven_buffer_lock
 *
 */
void readTime(struct raven_event_buffer *buffer, buffer_length_t reader_index, ktime_t *event_time) {
    ktime_t *time_index = &buffer->time_array[reader_index];
#if BITS_PER_LONG >= 64
    smp_read_barrier_depends(); // read barrier depends is necessary because reader_index could be the writer index which can be changed by the writer
    *event_time=READ_ONCE(*time_index);
#else
    struct raven_buffer_lock lock;
    do {
        lock = read_array_seqbegin(&buffer->lock, reader_index);
        *event_time = *time_index;
    } while (unlikely(read_array_seqretry(&buffer->lock, reader_index, lock)));
#endif
}

/**
 * Reads an index from a buffer.
 *
 * Reads an index from a buffer atomically. Because each buffer index event_buffers_length is dynamic and the timestamp must match the data,
 * the read must be synchronized to ensure it is correct. To this end we use a custom
 * sequential lock, for more information on this lock please see the documentation on struct raven_buffer_lock.
 *
 * @warning the index must be valid otherwise an overflow will occur
 *
 * @see struct raven_buffer_lock
 *
 * @param buffer the buffer to read the index from
 * @param reader_index the index to read from the buffer
 * @param event_data a pointer to an array where the data read from the buffer will be saved to
 * @param event_time a pointer to a ktime_t where the read time will be saved to
 *
 * @return whether an event was read, guaranteed to be 1.
 */
static inline void readIndex(struct raven_event_buffer *buffer, buffer_length_t reader_index, char *event_data,
                             ktime_t *event_time) {
    ktime_t *time_index = &buffer->time_array[reader_index];
    unsigned short data_block_size = buffer->index_size;
    void *data_index = (void *) &buffer->data_array[reader_index * data_block_size];

    struct raven_buffer_lock lock;
    do {
        lock = read_array_seqbegin(&buffer->lock, reader_index);
        *event_time = *time_index;
        memcpy((void *) event_data, data_index, data_block_size);
    } while (unlikely(read_array_seqretry(&buffer->lock, reader_index, lock)));
}

/**
 * Finds the buffer tail, then finds the first element larger than the synchronization variable, pre advancing the index
 * for the next read, and updating the last_read timestamp.
 *
 * When elements we have not read have been overwritten from the buffer that are smaller than the synchronization
 * variable, there is a gap in the trace. Elements that we would have read have been overwritten thus the gap boolean is set.  We have also lost track of the buffers head and tail, we no longer know where the buffer starts and ends.
 *
 * It is necessary to find the buffers tail, where the oldest element is located, for example : [3,4,2], in this case 2
 * is the tail where the last element is.
 *
 * To do this we use a while loop to go backwards to find the first element that is larger than the afterTime
 * synchronization variable, that is:  until the next index is larger than the current index or the next
 * index is smaller than the afterRead synchronization variable.
 *
 *   \dot
 *      digraph popGap {
 *          node[shape=doublecircle]; 5;
 *          node[shape=circle]
 *          1->2 [label="Decrement Index"]
 *          2->3 [label="Read element at index"]
 *          3->1 [label="if element < than last read element and > than the synch variable"]
 *          3->4 [label="else"]
 *          4->5 [label="return last read element"]
 *      }
 *   \enddot
 *
 *
 *  Notice that going backwards is safe because:
 *
 * - If a writer writes as fast as we read, going forward could possibly take an infinite amount of time, but going
 * backwards guarantees convergence as each party goes in opposite directions.
 * - Even if a buffer overflows on an overflow, if the next element we read is larger than then one we current read,
 * we have definitively found the smallest element in the EventBuffer.
 *
 * @param buffer the buffer to read an event from
 * @param reader_index the last_read index, which will be updated with new last read index
 * @param gap set to 1 if there is a gap in the trace, that is if elements have been overwritten that we have not read yet considering the sync variable, otherwise it'll be set to 0.
 * @param empty set to true if the buffer is empty, guaranteed to be false
 * @param last_read a pointer to the last_read element
 * @param last_event_data a pointer to the last read event data which will be updated with the new read timestamp
 * @param last_event_time the time of the last read event
 * @param synch the last read synchronization variable
 *
 * @warning the index must be valid otherwise an overflow will occur
 *
 * @return whether an event was read, guaranteed to be 1.
 */

static inline char gap_synch(struct raven_event_buffer *buffer, buffer_length_t *reader_index, unsigned char *gap,
                             unsigned char *empty, struct timespec *last_read, char *last_event_data,
                             ktime_t last_event_time, ktime_t synch) {
    ktime_t event_time;
    char temp_data[MAX_INDEX_SIZE];

    if (*reader_index != 0) --*reader_index;
    else *reader_index = buffer->length - 1;

    readIndex(buffer, *reader_index, temp_data, &event_time);
    //find the first element that is larger than the afterTime synchronization variable
    while (ktime_compare(event_time, last_event_time) < 1 && ktime_compare(event_time, synch) == 1) {
        last_event_time = event_time;
        memcpy(last_event_data, temp_data, buffer->index_size);
        if (*reader_index != 0) --*reader_index;
        else *reader_index = buffer->length - 1;
        readIndex(buffer, *reader_index, temp_data, &event_time);
    }

    ktime_t last_overwritten;
    readLastOverwritten(buffer, &last_overwritten);

    //we increment the index for the next read, we must increment twice because we want the last_event_data index + 1.
    if (++*reader_index == buffer->length) *reader_index = 0;
    if (++*reader_index == buffer->length) *reader_index = 0;

    *empty = 0;
    if (ktime_compare(last_overwritten, synch) == 1)
        *gap = 1;
    else *gap = 0;
    *last_read = ktime_to_timespec(last_event_time);
    return 1;
}

/**
 * Pops the next element in the buffer that is larger than the sync variable.
 *
 * The pop operation pops the next element that is larger than the synch variable,
 * while checking for a gap and taking the necessary measures if there is a gap in the trace.
 *
 * The pop operation consists of the following steps:
 *
 * - First we check that there are elements in the buffer that are larger than the synchronization variable.
 *      -# If there are none, then we can move the index to the buffers tail and set gap=false, finally, we return false.
 *
 * - If there are, we read events until we find a timestamp that is larger than the synchronization variable, while
 *   updating the last read timestamp.
 *
 * - We read the last overwritten timestamp:
 *      -# If it is larger than the last read element, then a gap could have occurred, we call popGap to handle the
 *      problem for us and pop the correct element.
 *      -# If it is not,  we update the SynchronizedReader state variables, lastReadEvent, afterTime,
 *      pre advance the index for the next read , overwrite the data on the output variable event,
 *      set the gap bool, and return true.
 *
 * \dot
 *      digraph pop {
 *          node[shape=doublecircle]; 2 6 9;
 *          node[shape=circle]
 *
 *          1->2[label="If all elements in the Buffer < than the synch variable"]
 *          1->3[label="If there are elements in the buffer that are > than the synch variable"]
 *          3->4[label="Read an Element"]
 *          4->3[label="if timestamp <= than the synchronization timestamp"]
 *          4->5[label="read last overwritten"]
 *          5->6[label="if last overwritten > last_read"]
 *          6->7[label="return popGap"]
 *          5->8[label="If lastOverwritten is <= than the last read timestamp"]
 *          8->9[label="return element"]
 *      }
 * \enddot
 *
 * Please note that reading an index and reading the last_overwritten time stamp is
 * not an atomic operation. We might have read the next sequentially correct element,
 * but because of preemptions, a data race, by the time we read the
 * last_overwritten, it might be > than the last read. Thus a gap warning will be thrown.
 *
 * Caution should be given to the order of operations in the buffer as these are very important, otherwise data
 * races can occur that can leave the EventReader and the read data in an inconsistent state.
 *
 *
 * Notice that  the writer only updates the last written index  after writing the data. Readers always check first that
 * the last written element is larger than whatever time they are looking for. When they start reading from the buffer,
 * is is guaranteed that an element exists that that is larger than the synchronization variable and can be popped.
 * Unless an overflow occurs, a reader can never become locked because a writer is writing data.
 *
 * @param buffer the buffer to pop the event from
 * @param reader_index the last read index which will be updated with the new index
 * @param gap will be set to one if a gap occurs, that is if elements were overwritten from the buffer that we did not read that would end up being read according to the syncronization variable
 * @param empty a pointer to an empty char, which will be set to 1 if the buffer is empty, 0 otherwise
 * @param last_read_p a pointer to the last_read time stamp which will be updated with the new time stamp
 * @param event_data a pointer to an array where the new data will be saved
 * @param synch the synchronization variable which holds the last read time stamp
 *
 * @warning reader_index must point to a valid index otherwise an overflow will occur.
 *
 * @return true if an element was read, false if the RoundBuffer is empty.
 *
 * @see struct raven_buffer_lock
 */
char pop_synch(struct raven_event_buffer *buffer, buffer_length_t *reader_index, unsigned char *gap,
               unsigned char *empty, struct timespec *last_read_p, char *event_data, ktime_t synch) {
    buffer_length_t i = ACCESS_ONCE(buffer->index);
    ktime_t event_time;
    readTime(buffer, i, &event_time);
    if (ktime_compare(event_time, synch) < 1) {
        //there are no new elements in the buffer
        if (++i != buffer->length) *reader_index = i;
        else *reader_index = 0;
        *gap = 0;
        *empty = 1;
        *last_read_p = ktime_to_timespec(event_time);
        return 0;
    } else {
        ktime_t last_read = timespec_to_ktime(*last_read_p);
        //there are newer elements in the buffer
        readIndex(buffer, *reader_index, event_data, &event_time);
        while (ktime_compare(event_time, synch) < 1) {
            last_read = event_time;
            if (++*reader_index == buffer->length)
                *reader_index = 0;
            readIndex(buffer, *reader_index, event_data, &event_time);
        }

        ktime_t last_overwritten;
        readLastOverwritten(buffer, &last_overwritten);
        if (ktime_compare(last_overwritten, last_read) == 1) //we have lost elements
            return gap_synch(buffer, reader_index, gap, empty, last_read_p, event_data, event_time,
                             synch); //we need to find the buffers tail

        if (++*reader_index == buffer->length)//increment the index for the next read
            *reader_index = 0;

        *gap = 0;
        *empty = 0;
        *last_read_p = ktime_to_timespec(event_time);
        return 1;
    }
}


/**
 * Pops the newest event from the buffer.
 *
 * PopHead requests the current the current writer index from the buffer, and checks that the event is newer than
 * the synchronization synch variable, popping the element if this condition is true.
 *
 * Pop head also checks for a gap in the trace, that is if the last overwritten element is larger than the afterRead
 * variable.
 *
 * @param buffer the buffer to pop the element from
 * @param reader_index the last read index which will be updated with the new index
 * @param gap true if elements have been overwritten that are larger than the synch variable, that is if we missed elements elements we would have otherwise read
 * @param empty true (1) if the buffer is empty, false (0) if it is not
 * @param last_read_p a pointer to the last read timestamp which will be updated with the new timestamp.
 * @param event_data a pointer to a memory section where the event data will be written to
 * @param synch the synchronization variable used for this event reader, can be the last read event if this buffer is not synchronized.
 *
 * @return true if an element was read.
 */
char pop_head(struct raven_event_buffer *buffer, buffer_length_t *reader_index, unsigned char *gap,
              unsigned char *empty, struct timespec *last_read_p, char *event_data, ktime_t synch) {
    ktime_t event_time;
    buffer_length_t i = ACCESS_ONCE(buffer->index);

    readIndex(buffer, i, event_data, &event_time);
    if (ktime_compare(event_time, synch) > 0) {
        ktime_t last_overwritten;
        readLastOverwritten(buffer, &last_overwritten);
        //there are newer elements in the buffer
        if (ktime_compare(last_overwritten, synch) == 1)
            *gap = 1;
        else
            *gap = 0;
        *empty = 0;
        *last_read_p = ktime_to_timespec(event_time);
        (++i != buffer->length) ? *reader_index = i : (*reader_index = 0);//increment the index for the next read
        return 1;
    } else {
        //there are no newer elements in the buffer
        *empty = 1;
        *gap = 0;
        *last_read_p = ktime_to_timespec(event_time);
        (++i != buffer->length) ? *reader_index = i : (*reader_index = 0); //increment the index for the next read
        return 0;
    }
}

/**
* Pushes an event to the Buffer in constant O(1) time, using MONOTONIC clock as a timestamp reference.
 *
 * Pushes events to the buffer in O(1) time using the monotonic clock as a timestamp reference.
 *
 * This method uses a custom sequential lock, for more information please check the struct raven_buffer_lock documentation.
 *
 * If the current architecture is x64 then ktime_t, which has event_buffers_length of 8 bytes can fit inside a register and the write is
 * atomic. Still one must be careful to not let the compiler  fold, spindle, or otherwise mutilate accesses.
 *
 * Otherwise the write must be synchronized using a seqlock to ensure it is correct
 *
 * Notice that  the writer only updates the last written index  after writing the data. Readers always check first that
 * the last written element is larger than whatever time they are looking for. When they start reading from the buffer,
 * is is guaranteed that an element exists that that is larger than the synchronization variable and can be popped.
 * Unless an overflow occurs, a reader can never become locked because a writer is writing data.
 *
 * @param buffer a buffer to push an event to
 * @param event_data the event data to write to the buffer
 */
void push(struct raven_event_buffer *buffer, char *event_data) {

    struct raven_buffer_lock lock_begin = array_write_getindex(&buffer->lock, buffer->length);

    unsigned short data_block_size = buffer->index_size;
    ktime_t *time_i = &buffer->time_array[lock_begin.writer_index];
    char *buffer_i = buffer->data_array + (lock_begin.writer_index * data_block_size);

    ktime_t event_time = ktime_get_raw(); //use only monotonic clock, otherwise data races can occur.
    //https://www.kernel.org/doc/Documentation/ioctl/botching-up-ioctls.txt

    barrier();

#if BITS_PER_LONG >= 64
    WRITE_ONCE(buffer->last_overwritten,*time_i);
#else
    write_seqlock(&buffer->last_overwritten_lock);
    WRITE_ONCE(buffer->last_overwritten, *time_i);
    write_sequnlock(&buffer->last_overwritten_lock);
#endif

    array_write_seqbegin(lock_begin, &buffer->lock);
    memcpy(buffer_i, event_data, data_block_size);
    WRITE_ONCE(*time_i, event_time);
    array_write_seqend(&buffer->lock);

    WRITE_ONCE(buffer->index, lock_begin.writer_index); //update the writer index last
}

/**
 * Finds the buffers tail and pops the element at that index, advancing the index and updating the lastReadEvent timestamp.
 *
 * When elements have been overwritten from the buffer that we have not read, there is a gap in the trace and thus,
 * the gap bool is set. We lose track of the buffer head and tail, we no longer know where the buffer starts and ends.
 *
 * It is necessary to find where the buffer tail is,  where the oldest element is in the buffer, for example: [3,4,2],
 * where 2 is the tail.
 *
 * To do this we make use of a while loop that goes backwards until the next index is larger than the current index,
 * or index+1 > index. We overwrite the return variable event with that index+1, update the last read timestamp.
 * and increment to the next index.
 *
 *   \dot
 *      digraph popGap {
 *          node[shape=doublecircle]; 5;
 *          node[shape=circle]
 *          1->2 [label="Decrement Index"]
 *          2->3 [label="Read element at index"]
 *          3->1 [label="if element < than last read element"]
 *          3->4 [label="if element > than last read element"]
 *          4->5 [label="return last read element"]
 *      }
 *   \enddot
 *
 * Notice that going backwards is safe because:
 *
 * - If a writer writes as fast as we read, going forward could possibly take an infinite amount of time, but going
 * backwards guarantees convergence as each party goes in opposite directions.
 * - Even if a buffer overflows on overflows, if the next element we read is larger than then one we current read,
 * we have definitively found the smallest element in the EventBuffer.
 *
 * @param buffer the buffer to read an event from
 * @param reader_index the last_read index, which will be updated with new last read index
 * @param gap set to 1 if there is a gap in the trace, that is if elements have been overwritten that we have not read yet considering the sync variable, otherwise it'll be set to 0.
 * @param empty sets if the buffer is empty, guaranteed to be false
 * @param last_read_p a pointer to the last_read element
 * @param last_event_data a pointer to the last read event data which will be updated with the new read timestamp
 * @param last_event_time the time of the last read event
 *
 * @warning the index must be valid otherwise an overflow will occur
 *
 * @return whether event was read, guaranteed to be true.
 */
static inline char gap_pop(struct raven_event_buffer *buffer, buffer_length_t *reader_index, unsigned char *gap,
                           unsigned char *empty, struct timespec *last_read_p, char *last_event_data,
                           ktime_t last_event_time) {
    ktime_t event_time;
    char temp_data[MAX_INDEX_SIZE];

    if (*reader_index != 0)--*reader_index;
    else *reader_index = buffer->length - 1;

    readIndex(buffer, *reader_index, temp_data, &event_time);
    //go back until we find the smallest event in the buffer, as we have overflowed this is always the element we are
    // looking for
    while (ktime_compare(event_time, last_event_time) < 0) {
        last_event_time = event_time;
        memcpy(last_event_data, temp_data, buffer->index_size);
        if (*reader_index != 0)--*reader_index;
        else *reader_index = buffer->length - 1;
        readIndex(buffer, *reader_index, temp_data, &event_time);
    }

    //we increment the index for the next read, we must increment twice because we want the last_event_data index + 1.
    if (++*reader_index == buffer->length) *reader_index = 0;
    if (++*reader_index == buffer->length) *reader_index = 0;

    *empty = 0;
    *gap = 1;
    *last_read_p = ktime_to_timespec(last_event_time);
    return 1;
}

/**
 * Pops the next event from the buffer.
 *
 * The pop operation pops the next element in the buffer that is larger than the lastReadEvent, while checking
 * for a gap in the buffer and taking the necessary measures if there is a gap in the trace.
 *
 * The pop operation coReader/writer consistent mechanism without starving writers. This type of lock is for data where the reader wants a
 * consistent set of information and is willing to retry if the information changes. The purpose here is to give writers
 * O(1) constant write time.
 *
 * The pop operation consists of the following steps:
 *
 * - Read the current index.
 *   If the timestamp is smaller, we set gap to false and return false, indicating that there is no gap in the trace and that the
 *   event object was not updated, thus no objects were popped.
 *
 *  - If the element's timestamp is larger than the last_read the buffer is not empty and we read the last_overwritten timestamp:
 *
 *      -# If the last overwritten timestamp is larger than the last read event in this buffer, then we know a gap might
 *      have occurred. We find the buffers tail, for details on this operation see popGap method.
 *
 *      -# If it is smaller or equal, then we haven't missed any events. We overwrite events data with the current
 *      index data, save its timestamp as the last_read, and increase index by one for the next read operation.
 *
 *      \dot
 *      digraph pop {
 *          node[shape=doublecircle]; 3 7 9;
 *          node[shape=circle]
 *          1->2 [label="Read the index"]
 *          2->3 [label="If index timestamp <= than last_read"]
 *          2->4 [label="If index > than the last_read"]
 *          4->5 [label="read the last overwritten timestamp"]
 *          5->6 [label="if we have lost elements"]
 *          6->7 [label="call popGap"]
 *          5->8 [label="if we have not lost elements"]
 *          8->9 [label="return read element"]
 *      }
 *      \enddot
 *
 *
 * Please note that reading an index and reading the last_overwritten time stamp is
 * not an atomic operation. We might have read the next sequentially correct element,
 * but because of preemptions or a mere data race, by the time we read the
 * last_overwritten, it might be > than the last read. Thus a gap warning will be thrown.
 *
 *
 *  Caution should be given to the order of operations in the buffer as these are very important, otherwise data
 *  races can occur that can leave the event reader and the read data in an inconsistent state.
 *
 * Notice that  the writer only updates the last written index  after writing the data. Readers always check first
 * that the last written element is larger than whatever time they are looking for. When they start reading from the\
 * buffer, is is guaranteed that an element exists that that is larger than the synchronization variable and can be popped.
 * Unless an overflow occurs, a reader can never become locked because a writer is writing data.
 *
 *
 * @param buffer The buffer to pop the event from
 * @param reader_index a pointer to the the last read index which will be updated with the new index
 * @param gap will be set to one if a gap occurs, that is if elements were overwritten from the buffer that we did not read that would end up being read according to the synchronization variable
 * @param empty a pointer to an empty char, which will be set to 1 if the buffer is empty, 0 otherwise
 * @param last_read_p a pointer to the last_read time stamp which will be updated with the new time stamp
 * @param event_data a pointer to an array where the new data will be saved
 *
 * @warning reader_index must be within range otherwise an overflow will occur.
 *
 * @return true if an element was read, false if the RoundBuffer is empty.
 */
char pop(struct raven_event_buffer *buffer, buffer_length_t *reader_index, unsigned char *gap, unsigned char *empty,
         struct timespec *last_read_p, char *event_data) {
    ktime_t last_read = timespec_to_ktime(*last_read_p);
    ktime_t event_time;
    readIndex(buffer, *reader_index, event_data, &event_time);

    if (ktime_compare(event_time, last_read) == 1) {
        ktime_t last_overwritten;
        readLastOverwritten(buffer, &last_overwritten);
        if (ktime_compare(last_overwritten, last_read) == 1) //we have lost elements
            return gap_pop(buffer, reader_index, gap, empty, last_read_p, event_data,
                           event_time); //we need to find the buffers tail

        //increment the index for the next read
        if (++*reader_index == buffer->length)
            *reader_index = 0;

        *gap = 0;
        *empty = 0;
        *last_read_p = ktime_to_timespec(event_time);
        return 1;
    } else {
        *gap = 0;
        *empty = 1;
        return 0;
    }
}

#endif //KERNEL_ARRAY_OPERATIONS_H
