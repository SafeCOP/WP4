#ifndef MONITOR_ARRAYSEQLOCK_H
#define MONITOR_ARRAYSEQLOCK_H

#include <boost/atomic.hpp>

using namespace boost;

#include "types.h"

/**
 *Current writer state: what index it's on, is that index being updated, and how many times it has wrapped the buffer.
 *
 * This structure size is dependant on the architecture, on X32 it is 4 bytes long, on x64 it is 8 bytes long, thus
 * updates to this variable are atomic if properly instructed.
 *
 * The reason why the structure definition is dependant on architecture is to allow the buffers to be as big as possible.
 */
struct array_seq_lock {
    /** The index the writer is writing at */
    length_t writer_index;
#if BITS_PER_LONG >= 64
    /** Number of times the buffer has wrapped, or how many times it passed buffer_length-1*/
    unsigned short wrap_count;
#else
    /** Number of times the buffer has wrapped, or how many times it passed buffer_length-1*/
    unsigned char wrap_count;
#endif
/** True if the index pointed by writer_index is being written to */
    bool writing;
};

/**
 * Reader/writer consistent mechanism without starving writers that protects an entire array where a single writer exists.
 * This type of sequence is for data where the reader wants a consistent set of information and is willing to retry if
 * the information changes. The purpose here is to give writers O(1) constant write time.
 *
 * Writer usage example:
 * @code
 * length_t index = arraySeqLock.write_get_index();
 * Array * tmp=array[index];
 * arraySeqLock.write_seqbegin();
 * tmp=data;
 * arraySeqLock.write_seqend();
 * @endcode
 *
 * Reader usage example:
 * @code
 *  struct array_seq_lock lock;
 *  do{
 *      lock = arraySeqLock.read_array_seqbegin(reader_index);
 *      ....
 *  } while(arraySeqLock.read_array_seqretry(lock, reader_index));
 * @endcode
 *
 * The writing algorithm:
 *
 *  - Set the current writer index, and writing to true, incrementing the wrap_count if the buffer has wrapped
 *  (if the index being written to went from length-1 to 0), on a local variable seen only by the writer.
 *  - Update the global variable seen by readers atomically.
 *  - Issue a memory and compiler barrier.
 *  - Update the data
 *  - Issue a memory and compiler barrier.
 *  - Sets writing to false atomically on the global variable using WRITE_ONCE.
 *
 *
 *  The reading algorithm:
 *  - Read the sequence structure atomically into lock1.
 *  - If the writer is writing on the index we wish to read, busy wait until the writer is finished.
 *  - Issue a memory and compiler barrier.
 *  - Read data from the array.
 *  - Issue a memory and compiler barrier.
 *  - Read the sequence structure atomically into lock2.
 *  - Figure out if a writer overlapped while we were reading:
 *     -# If the wrap count is the same, then the only situation that results in an overlap is when the writer was writing on an index that was smaller than the one we were reading before we read the element, and writing on a larger or equal index than the index we were reading, after we finished reading. That is lock1.wrap_count == lock2.wrap_count and lock1.writer_index < reading_index and lock2.writer_index>=reading_index.
 *     -# If the wrap count on lock2 is lock1+1, then we must ensure that the writer has yet to write over our index, that is lock2.wrap_count == lock1.wrap_count +1 and sequence.writer_index >= reader_index and lock2.writer_index < reading_index
 *     -# If all above fail, a writer has overlapped and we must begin this cycle again.
 *
 * Atomic operations are provided by boost::atomic, which replicates the C++11 memory
 * model. It is possible to change implementations by merely changing the include and
 * the namespace.
 *
 * This class could wrap all push/pop operations to the array by means of a template class.
 * However this would prevent partial access to the array, for example if we only wanted to do a partial read to an index, it would be impossible to do so.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
class ArraySeqLock {
private:
    /**
     * The atomic sequence the writers/readers check for.
     */
    atomic<struct array_seq_lock> sequence;

    /**
     * A cached version of sequence used to return the index by write_get_index to avoid
     * locking the array before actually writing to it.
     */
    struct array_seq_lock tmp_sequence;

    /**
     * The length of the array we protect.
     */
    const length_t array_length;

public:
    /**
     * Instantiate a new ArraySeqLock to protect an array of length array_length.
     *
     * @param array_length the length of the array to protect
     */
    ArraySeqLock(const length_t array_length);

    /**
     * Gets the next index to write on the protected buffer.
     *
     * @return the index to write to the protected buffer.
     */
    length_t write_get_index();

    /**
     * Begins a write critical section.
     *
     * Issues a full memory barrier, memory operations or instructions will not be reordered before/past this point.
     */
    void write_seqbegin();

    /**
     * Ends a write critical section.
     *
     * Issues a full memory barrier, memory operations or instructions will not be reordered before/past this point.
     */
    void write_seqend();

    /**
     * Begins a read side critical section.
     *
     * Issues a full memory barrier, memory operations or instructions will not be reordered before/past this point.
     *
     * @param index the index to read from the protected buffer.
     *
     * @return the sequence state before reading, to be used with a corresponding read_seqretry
     */
    struct array_seq_lock read_seqbegin(const length_t index) const;

    /**
     * Ends a read side critical section.
     *
     * Issues a full memory barrier, memory operations or instructions will not be reordered before/past this point.
     *
     * @param seq_lock the writer sequence state before reading from the buffer, initialized by read_seqretry()
     * @param index the index to read from the protected buffer.
     *
     * @return true if we need to read again
     */
    bool read_seqretry(const struct array_seq_lock &seq_lock, const length_t index) const;

    /**
     * Returns the length of the protected array.
     */
    length_t getLength() const;
};


#endif //MONITOR_SEQLOCK_H
