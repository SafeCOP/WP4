#ifndef KERNEL_EVENTREADER_H
#define KERNEL_EVENTREADER_H

#include <sys/types.h>
#include "../c/raven_api.h"
#include "IEventReader.h"

/**
 * C++ wrapper api for EventReaders.
 *
 *  Event Reader reads Events from an EventBuffer FIFO fashion. EventReader does what it can when elements
 * are written faster than it can read. When overflows occurs EventReader tries to find the oldest element it has yet to
 * read and notifies the user of the overflow.
 *
 * EventReaders are instantiated or configured by EventBuffers or by the EventBufferFactory.
 *
 * @see EventBuffer
 * @see EventBufferFactory
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
template<class T>
class EventReader : public IEventReader<T> {
public:
    /**
     * Instantiates a new EventReader.
     */
    EventReader();

    /**
     * Instantiates a new EventReader
     *
     * @warning The buffer id is not validated.
     *
     * @param buffer_id the buffer id to configure this EventReader to.
     */
    EventReader(const key_t buffer_id);

    /**
     * Configures this EventReader to push events to the buffer with the id specified as param.
     *
     * @warning no checks are made to ensure that the EventBuffer exists or that its index length matches the template type T.
     *
     * @arg buffer_id the id of te buffer to configure this EventReader to.
     */
    void configure(const key_t buffer_id);

    /**
     * Checks if the buffer is empty.
     *
     * Calls raven buffer info to get the timestamp of the last written element. It then compares it to the last read timestamp to check if it is newer.
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
EventReader<T>::EventReader() : IEventReader<T>() {}

template<class T>
EventReader<T>::EventReader(const key_t buffer_id) : IEventReader<T>(buffer_id) {}

template<class T>
void EventReader<T>::configure(const key_t buffer_id){
    raven_init_event_reader(&this->reader,buffer_id,NULL,NULL);
}

template<class T>
int EventReader<T>::emptyBuffer() const {
    struct event_buffer_info info;
    int op=raven_buffer_info(this->reader.id, &info);
    if(op==-1) return -1;

    if(info.last_written.tv_sec <= this->reader.last_read.tv_sec && info.last_written.tv_nsec <= this->reader.last_read.tv_nsec)
        return 1;
    else
        return 0;
}
#endif //KERNEL_EVENTREADER_H

