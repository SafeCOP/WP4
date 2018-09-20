#ifndef _MONITOR_BUFFER_H_
#define _MONITOR_BUFFER_H_

#include <time.h>

#include <boost/atomic.hpp>

using namespace boost;

#include "types.h"
#include "Event.h"
#include "SeqLock.h"
#include "ArraySeqLock.h"
#include "pthread.h"

/**
 * Data container for EventBuffer operators such as EventReader and EventWriter.
 *
 * Buffer is a Data Container for sharing buffer state, it contains data both readers and writers need.  Buffer also
 * defines some operations that can be done on the buffer, such as atomic reading and writing.
 *
 * This class exists as to avoid cyclic dependency where readers and writers would have to know the EventBuffer and
 * the EventBuffer having to know reader and writers.
 * Buffer also allows reader/writers to know only one object, instead of having several pointers to the data they need.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
template<typename T>
class Buffer {
private:
    /**
     * The sequence used to protect read/writes.
     */
    ArraySeqLock array_lock;

    /** Last overwritten element timestamp.
     * @see time.h
     */
    SeqLock<struct timespec> lastOverwrittenTimestamp;

    /** Last written index.
     */
    atomic<length_t> index;

    /** Constant pointer to the start of the array */
    Event<T> *const array;

public:
    /** Array Length */
    const length_t length;

    /**
     * Instantiates a new buffer.
     *
     * The buffer begins at array and has a length of length. The lastOverwrittenTimestamp is zeroed out.
     *
     * @param array a reference to a constant pointer that points to the array.
     * @param length the length of the array.
     */
    Buffer(Event<T> *const &array, const length_t length);

    /**
     * Pushes an event to this circular buffer.
     *
     * Pushes events to the buffer in O(1) time using the monotonic clock as a timestamp reference.
 *
 * This method uses a custom sequential lock, for more information please check the ArraySeqLock and the SeqLock documentation.
     *
     * Since the index is always updated after writing an element, unless an overflow occurs the readers should never try
     * to read this element, as readers always call getHead to obtain the current index.
     *
     * @param event the event to be pushed
     */
    void push(const Event<T> &event);

    /**
     * Pushes data into this circular buffer.
     *
     *  Pushes events to the buffer in O(1) time using the monotonic clock as a timestamp reference.
     *
     * This method uses a custom sequential lock, for more information please check the ArraySeqLock and the SeqLock documentation.
     *
     * Since the index is always updated after writing an element, unless an overflow occurs the readers should never try
     * to read this element, as readers always call getHead to obtain the current index.
     *
     * @param data the data to be pushed.
     */
    void push(const T &data);

    /**
     * Atomically reads an index from the buffer.
     *
     *  @warning the index must be valid otherwise an overflow will occur
     *
     * @param event A reference to an event object where the data will be stored.
     * @param index the index to read from.
     */
    void readIndex(Event<T> &event, const length_t index) const;

    /**
     * Atomically reads the lastOverwritten timestamp.
     *
     * @param last_overwritten  a reference to a struct timespec where the last_overwritten will be read to.
     */
    void readLastOverwritten(struct timespec &last_overwritten) const;

    /**
     * Atomically reads a timestamp from an index.
     *
     * If we fail to read the index after 5 tries, pthread_yield is called to yield to the writer thread.
     *
     * @warning the index must be valid otherwise an overflow will occur
     *
     * @param time A reference to a struct timespec where the data will be stored.
     * @param index the index to read from.
     */
    void readTime(struct timespec &time, const length_t index) const;

    /**
     * Atomically reads the last written index.
     *
     * If we fail to read the index after 5 tries, pthread_yield is called to yield to the writer thread.
     *
     * @warning the index must be valid otherwise an overflow will occur
     *
     * @param time A reference to a struct timespec where the data will be stored.
     * @param index the index to read from.
     *
     * @return the last written index
     */
    length_t readLastWritten(struct timespec &time) const;

    /**
     * Gets the buffers head, the index where the writer last wrote.
     *
     * @return the index that was written.
     */
    length_t getHead() const;

    /**
     * Gets the array length.
     *
     * @return the array length.
     */
    length_t getLength() const;

    /**
     * Checks if the array is empty.
     *
     * Since the index to be written is index[1], this method checks if the
     * Event on that index is null.
     *
     * @return if the array is empty.
     */
    bool empty() const;
};

template<typename T>
Buffer<T>::Buffer(Event<T> *const &array, const length_t length) : array_lock(length), lastOverwrittenTimestamp(),
                                                                   index(0), array(array), length(length) { }

template<typename T>
void Buffer<T>::readTime(struct timespec &time, const length_t index) const {
    struct array_seq_lock tmp_lock;
    do {
        tmp_lock = array_lock.read_seqbegin(index);
        time = array[index].getTime();
    }while(array_lock.read_seqretry(tmp_lock, index));
}


template<typename T>
void Buffer<T>::push(const Event<T> &event) {
    /* We must increment the writer_index on a temporary variable,
    * otherwise if a reader calls getHead() he might get an index
    * which is > than the array.
    */

    const length_t tempIndex = array_lock.write_get_index();
    Event<T> *tempEvent = &array[tempIndex];

    lastOverwrittenTimestamp.store(tempEvent->getTime());

    array_lock.write_seqbegin();
    *tempEvent = event;
    array_lock.write_seqend();

    //Since the index is updated last, readers should never come across this element unless they overflow. Because
    //the readers always call getHead() and check if that time > than lastRead or > than afterTime.
    index.store(tempIndex, memory_order_relaxed);
};

template<typename T>
void Buffer<T>::push(const T &data) {
    /* We must increment the writer_index on a temporary variable,
    * otherwise if a reader calls getHead() he might get an index
    * which is > than the array.
    */
    struct timespec tmp_time;
    clock_gettime(CLOCK_MONOTONIC, &tmp_time);

    const length_t tempIndex = array_lock.write_get_index();
    Event<T> *tempEvent = &array[tempIndex];

    lastOverwrittenTimestamp.store(tempEvent->getTime());

    array_lock.write_seqbegin();
    tempEvent->getTime() = tmp_time;
    tempEvent->getData() = data;
    array_lock.write_seqend();

    //Since the index is updated last, readers should never come across this element unless they overflow. Because
    //the readers always call getHead() and check if that time > than lastRead or > than afterTime.
    index.store(tempIndex, memory_order_relaxed);
}

template<typename T>
void Buffer<T>::readIndex(Event<T> &event, const length_t index) const {
    struct array_seq_lock tmp_lock;
    do {
        tmp_lock = array_lock.read_seqbegin(index);
        event = array[index];
    } while (array_lock.read_seqretry(tmp_lock, index));
}

template<typename T>
void Buffer<T>::readLastOverwritten(struct timespec &last_overwritten) const {
    lastOverwrittenTimestamp.load(last_overwritten);
}

template<typename T>
length_t Buffer<T>::readLastWritten(struct timespec &time) const {
    struct array_seq_lock tmp_lock;
    length_t tmp_index;
    do {
        tmp_index = this->index.load(memory_order_acquire);
        tmp_lock = array_lock.read_seqbegin(tmp_index);
        time = array[tmp_index].getTime();
    } while(array_lock.read_seqretry(tmp_lock, tmp_index));

    return tmp_index;
}

template<typename T>
length_t Buffer<T>::getHead() const {
    return index.load(memory_order_relaxed);
}

template<typename T>
length_t Buffer<T>::getLength() const {
    return length;
}

template<typename T>
bool Buffer<T>::empty() const {
    struct timespec time;
    readTime(time, 1);
    return time.tv_nsec == 0 && time.tv_sec == 0;
}

#endif //_MONITOR_BUFFER_H_
