#ifndef KERNEL_SYNCHRONIZEDEVENTREADER_H
#define KERNEL_SYNCHRONIZEDEVENTREADER_H

#include <sys/types.h>
#include "IEventReader.h"

/**
 * C++ wrapper for SynchronizedEventReaders.
 *
 * SynchronizedEventReader reads events from an EventBuffer while using a shared timeStamp variable between other
 * SynchronizedEventReaders. Events popped are always larger than this shared timestamp, in FIFO order.
 *
 * SynchronizedEventReader does what it can in situations where events are written faster than it can read them. The reader
 * always finds the first element larger than the shared synchronization variable to read next.
 *
 * SynchronizedEventReaders are instantiated or configured by EventBuffers or EventBuffersFactory.
 * @see EventBuffer
 * @see EventBufferFactory
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
template<class T>
class SynchronizedEventReader : public IEventReader<T>{
public:
    /**
     * Instantiates an SynchronizedEventReader.
     */
    SynchronizedEventReader();

    /**
     * Instantiates a new SynchronizedEventReader.
     *
     * @warning the buffer id is not validated.
     *
     * @param buffer_id the buffer id to configure this Synchronized Event Reader to.
     * @param synch the synchronization variable to synchronize this EventBuffer.
     */
    SynchronizedEventReader(const key_t buffer_id, const struct timespec * synch);

    /**
     * Configures this synchronized reader to push events to a new EventBuffer and using a new synchronization variable.
     *
     * @warning the buffer id is not validated.
     *
     * @param id the id of the EventBuffer to push events to.
     * @param synch a pointer to the synchronization variable.
     */
    void configure(key_t id, struct timespec * synch);


    /**
      * Checks if the buffer is empty.
      *
      * Calls raven buffer info to get the timestamp of the last written element. It then compares it to the synchronization timestamp to check if it is newer.
      *
      * @return On error -1 is returned and errno is set accordingly. Else 1 if the buffer is empty, 0 if it is not.
      *
      *  errno error list:
      *  - ENOENT : The buffer id at event_buffer does not exist.
      *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
      *  - EFAULT : buffer_list references an inaccessible memory area.
      *  - EINVAL : Request or buffer_list is not valid. Never thrown inside the module, likely you forgot to initialize via raven_init().
      *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
      *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
      *
      *  Please consult your linux manual to see what errno codes ioctl might produce.
      */
    int emptyBuffer() const;
};

template<class T>
SynchronizedEventReader<T>::SynchronizedEventReader() : IEventReader<T>() { }

template<class T>
SynchronizedEventReader<T>::SynchronizedEventReader(const key_t buffer_id, const struct timespec *synch) : IEventReader<T>(buffer_id,synch) { }

template<class T>
void SynchronizedEventReader<T>::configure(key_t buffer_id, struct timespec * synch){
    raven_init_event_reader(&this->reader,buffer_id,NULL,synch);
}

template<class T>
int SynchronizedEventReader<T>::emptyBuffer() const {
    struct event_buffer_info info;
    int op=raven_buffer_info(this->reader.id, &info);
    if(op==-1) return -1;

    struct timespec synch = *this->reader.synch;

    if(info.last_written.tv_sec <= synch.tv_sec && info.last_written.tv_nsec <= synch.tv_nsec)
        return 1;
    else
        return 0;
}

#endif //KERNEL_SYNCHRONIZEDEVENTREADER_H
