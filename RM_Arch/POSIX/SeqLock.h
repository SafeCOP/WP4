//
// Created by silverone on 8/11/15.
//

#ifndef MONITOR_SEQLOCK_H
#define MONITOR_SEQLOCK_H

#include <boost/atomic.hpp>
#include <pthread.h>

using namespace boost;

/**
 * Reader/writer consistent mechanism without starving writers. This type of sequence is for data where the reader wants a
 * consistent set of information and is willing to retry if the information changes. The purpose here is to give writers O(1) constant write time.
 *
 *  The writing algorithm:
 *      - Increment sequence by 1 atomically so that it becomes an opposite number.
 *      - Issue a memory and compiler barrier.
 *      - The writer writes to the protected structure.
 *      - Issue a memory and compiler barrier.
 *      - The writer increments sequence by 1 so that it becomes a pair number again.
 *
 *  The reading algorithm:
 *      - The reader reads sequence atomically into s1.
 *      - Busy wait while s1 is not a pair number.
 *      - Issue a memory and compiler barrier.
 *      - The reader reads data from the array.
 *      - Issue a memory and compiler barrier.
 *      - The reader reads sequence atomically into s2.
 *      - If s1 != s2 we repeat the whole process.
 *
 * Atomic operations are provided by boost::atomic, which replicates the C++11 memory
 * model. It is possible to change implementations by merely changing the include and
 * the namespace.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
template<class T>
class SeqLock {
private:
    /**
     * The protected data.
     */
    T data;

    /**
     * The data update sequence number. An opposite number means the
     * data is being updated by a writer.
     */
    atomic<unsigned int> sequence;

    /**
     * Begins a write critical section by incrementing the sequence number to an opposite number.
     */
    void write_seqbegin();

    /**
     * Ends a write critical section by incrementing the sequence number to a pair number.
     */
    void write_seqend();

    /**
     * Begins a read critical section by reading the sequence number and busy waits
     * if the writer is currently updating the data.
     *
     * @return the write sequence number.
     */
    unsigned int read_seqbegin() const;

    /**
     * Ends a read critical section.
     *
     * Checks if the sequence number matches the one read by read_seqbegin().
     *
     * @returns true if we must retry th read.
     */
    bool read_seqretry(const unsigned int tmp) const;

public:
    /**
     * Instantiates a new SeqLock.
     *
     * Calls the default constructor on the Data.
     */
    SeqLock();

    /**
     * Instantiates a new Seqlock.
     *
     * Initializes the proctected data by copy constructor.
     *
     * @param data an object of type T to initialize data to.
     */
    SeqLock(const T &data);

    /**
     * Atomically reads the protected data.
     *
     * @param data An objec to read th data to.
     */
    void load(T &data) const;

    /**
     * Atomically replaces the contained data with data.
     *
     * @param data the data to replace the contained version with.
     */
    void store(const T &data);

    /**
     * Atomically replaces the contained data with data.
     *
     * @param data the data to replace the contained version with.
     */
    SeqLock &operator=(const T &data);
};

template<class T>
SeqLock<T>::SeqLock() : data(), sequence(0) { }

template<class T>
SeqLock<T>::SeqLock(const T &data) : data(data), sequence(0) { }

template<class T>
void SeqLock<T>::load(T &data) const {
    unsigned int lock;
    do {
        lock = read_seqbegin();
        data = this->data;
    }while (read_seqretry(lock));
}

template<class T>
void SeqLock<T>::store(const T &data) {
    write_seqbegin();
    this->data = data;
    write_seqend();
}

template<class T>
void SeqLock<T>::write_seqbegin() {
    unsigned int tmp = sequence.load(memory_order_relaxed) + 1;
    sequence.store(tmp, memory_order_seq_cst);
}

template<class T>
void SeqLock<T>::write_seqend() {
    unsigned int tmp = sequence.load(memory_order_relaxed) + 1;
    sequence.store(tmp, memory_order_seq_cst);
}

template<class T>
unsigned int SeqLock<T>::read_seqbegin() const {
    unsigned int attempts=0;
    unsigned int tmp;
    do {
        if(attempts++==20) {
            attempts=0;
            pthread_yield();
        }
        tmp = sequence.load(memory_order_relaxed);
    }while (tmp & 1);
    atomic_thread_fence(memory_order_seq_cst);
    return tmp;
}

template<class T>
bool SeqLock<T>::read_seqretry(const unsigned int tmp) const {
    unsigned int tmp2 = sequence.load(memory_order_seq_cst);
    return tmp2 != tmp;
}

template<class T>
SeqLock<T> &SeqLock<T>::operator=(const T &data) {
    store(data);
}

#endif //MONITOR_SEQLOCK_H

