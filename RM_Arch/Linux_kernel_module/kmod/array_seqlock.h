/**
 * @file array_seqlock.h
 *
 * This file declares the array seq lock mechanism. A writer priority array reader/writer synchronization mechanism.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */

#ifndef KERNEL_ARRAY_SEQ_LOCK_H
#define KERNEL_ARRAY_SEQ_LOCK_H

#if BITS_PER_LONG >= 64
    /**
     * The maximum buffer length. Type is chosen due to atomic updating limits. On 64 bit architectures up to 8b can be changed atomically.
     * The limit here is because of the ArraySeqLock mechanism which must include an index with some more data.
     */
    typedef unsigned int buffer_length_t;
#else
    /**
     * The maximum buffer length. Type is chosen due to atomic updating limits. On 32 bit architectures up to 4b can be changed atomically.
     * The limit here is because of the ArraySeqLock mechanism which must include an index with some more data.
     */
    typedef unsigned short buffer_length_t;
#endif


/**
 * Reader/writer consistent mechanism without starving writers. This type of lock is for data where the reader wants a
 * consistent set of information and is willing to retry if the information changes. The purpose here is to give writers
 * O(1) constant write time.
 *
 * This structure event_buffers_length is dependant on the architecture, on X32 it is 4 bytes long, on x64 it is 8 bytes long, thus updates to this variable are atomic if properly instructed. This is to accommodate larger buffers.
 *
 * Expected writer usage:
 *
 * struct raven_buffer_lock lock_begin=array_write_getindex(&buffer->lock, buffer->length);
 * array_write_seqbegin(lock_begin,&buffer->lock);
 * ....
 * array_write_seqend(&buffer->lock);
 *
 * Expected reader usage:
 * @code
 *  struct raven_buffer_lock lock;
 *  do{
 *      read_array_seqbegin(buffer, reader_index, &lock);
 *      ....
 *  } while(read_array_seqretry(buffer, reader_index, &lock));
 * @endcode
 * The writing algorithm:
 *
 *  - Writer sets the current writer index, and writing to true, incrementing the wrap_count if the buffer has wrapped
 *  (if the index being written to went from length-1 to 0), on a local variable seen only by the writer.
 *  - Update the global variable seen by readers atomically using WRITE_ONCE.
 *  - Call smp_wmb() to issue a write memory barrier.
 *  - Writer then writes the data
 *  - Call smp_wmb() to issue a write memory barrier.
 *  - the writer sets writing to false atomically on the global variable using WRITE_ONCE.
 *
 *
 *  The reading algorithm:
 *  - The reader reads the lock structure atomically into lock1 using READ_ONCE.
 *  - If the writer is writing on the index we wish to read, we live lock until the writer is finished.
 *  - call smp_rmb() to issue a read memory barrier.
 *  - The reader reads data from the array.
 *  - call smp_rmb() to issue a read memory barrier.
 *  - The reader reads the lock structure atomically into lock2 using READ_ONCE.
 *  - The reader must then figure out if a writer overlapped with a reader:
 *     -# If the wrap count is the same, then the only situation that results in an overlap is when the writer was writing on an index that was smaller than the one we were reading before we read the element, and writing on a larger or equal index than the index we were reading, after we finished reading. That is lock1.wrap_count == lock2.wrap_count and lock1.writer_index < reading_index and lock2.writer_index>=reading_index.
 *     -# If the wrap count on lock2 is lock1+1, then we must ensure that the writer has yet to write over our index, that is lock2.wrap_count == lock1.wrap_count +1 and lock.writer_index >= reader_index and lock2.writer_index < reading_index
 *     -# If all above fail, a writer has overlapped and we must begin this cycle again.
 */
struct raven_buffer_lock {
    buffer_length_t writer_index; /** The index the writer last wrote */
#if BITS_PER_LONG >= 64
    unsigned short wrap_count; /** Number of times the buffer has wrapped, or how many times it passed buffer_length-1*/
#else
    unsigned char wrap_count; /** Number of times the buffer has wrapped, or how many times it passed buffer_length-1*/
#endif
    unsigned char writing; /** True if the index pointed by writer_index is being written to */
};

/**
 * Initializes a raven_buffer_lock.
 *
 * @param lock the lock to initialize.
 */
void array_seqlock_initialize(struct raven_buffer_lock * lock){
    lock->writer_index=0;
    lock->writing=0;
}

/**
 * Read function to start a read side section.
 *
 * @param lock_addr the address to the raven_buffer_lock to read the index from
 * @param reader_index the index to read from the buffer
 *
 * @see struct raven_buffer_lock for more information, and examples.
 */
struct raven_buffer_lock read_array_seqbegin(struct raven_buffer_lock *lock_addr,
                                                           buffer_length_t reader_index) {
    struct raven_buffer_lock lock = READ_ONCE(*lock_addr);
    while (unlikely(lock.writer_index == reader_index && lock.writing == 1)) {
        cpu_relax();
        lock = READ_ONCE(*lock_addr);
    }

    smp_rmb(); //read memory barrier

    return lock;
}

/**
 * Read function to finalize a read side section.
 *
 * @param lock_addr the address to the raven_buffer_lock to read the index from
 * @param reader_index the index to read from the buffer
 * @param lock a pointer to a lock structure that has been used with a corresponding read_array_seqbegin
 *
 * @see struct raven_buffer_lock for more information, and examples.
 */
unsigned char read_array_seqretry(struct raven_buffer_lock * lock_addr, buffer_length_t reader_index,
                                                struct raven_buffer_lock lock) {
    smp_rmb(); //read memory barrier
    struct raven_buffer_lock lock2 = READ_ONCE(*lock_addr);
    if (likely(lock.wrap_count == lock2.wrap_count)) {
        if (likely(!(lock.writer_index < reader_index && lock2.writer_index >= reader_index)))
            return 0;
    } else if (likely(lock.wrap_count + 1 == lock2.wrap_count)) {
        if (likely(lock.writer_index >= reader_index && lock2.writer_index < reader_index))
            return 0;
    }

    return 1;
}

/**
 * Gets the next index to write on the protected buffer.
 *
 * @param lock_addr the address to the raven_buffer_lock.
 * @param buffer_length the length of the buffer
 *
 * @return the index to write to the protected buffer.
 */
struct raven_buffer_lock array_write_getindex(struct raven_buffer_lock * lock_addr, buffer_length_t buffer_length){
    struct raven_buffer_lock lock=READ_ONCE(*lock_addr);
    if (++lock.writer_index == buffer_length) {
        lock.writer_index=0;
        ++lock.wrap_count;
    }
    lock.writing=1;

    return lock;
}

/**
 * Begins a write critical section.
 *
 * @param writer the writer_index provided by the write_get_index() function.
 * @param lock_addr the address to the raven_buffer_lock.
 *
 * Issues a write memory barrier, write memory operations or instructions will not be reordered before/past this point.
 */
void array_write_seqbegin(struct raven_buffer_lock  writer, struct raven_buffer_lock * lock_addr){
    WRITE_ONCE(*lock_addr, writer); //lock the buffer
    smp_wmb(); //write memory barrier
}

/**
 * Ends a write critical section.
 *
 * @param lock_addr the address to the raven_buffer_lock.
 *
 * Issues a read memory barrier, read memory operations or instructions will not be reordered before/past this point.
 */
void array_write_seqend(struct raven_buffer_lock * lock_addr){
    smp_wmb(); //write memory barrier
    WRITE_ONCE(lock_addr->writing, 0); //unlock the buffer
}
#endif //KERNEL_ARRAY_SEQ_LOCK_H
