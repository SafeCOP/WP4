//
// Created by silverone on 8/24/15.
//

#ifndef KERNEL_EVENTWRITER_H
#define KERNEL_EVENTWRITER_H

#include <sys/types.h>
#include "../c/raven_api.h"

/**
 * EventWriter C++ wrapper api to push events to KernelSide EventBuffers.
 *
 * Writing operations take O(1) time.
 *
 * EventWriters are instantiated by EventBuffers or by an EventBufferFactory.
 *
 * @see EventBuffer
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
template<class T>
class EventWriter {
private:
    /**
     * The event_writer struct associated with this buffer.
     */
    struct event_writer writer;

public:
    /**
     * Instantiate a new EventWriter.
     */
    EventWriter();

    /**
     * Instantiate a new EventWriter that writes to the EventBuffer with id id.
     *
     * @warning No checks are made to ensure the id is valid.
     */
    EventWriter(key_t id);

    /**
     * Pushes an event to an event buffer in O(1) constant time.
     *
     * Pushes the data to the event buffer via an ioctl call to /dev/raven.
     *
     * The event timestamp is generated from clock monotonic inside the kernel call. Only the buffers owner can push events to a buffer.
     *
     * @param data the data to push.
     *
     * @warning push is not synchronized internally, that means if you want multiple processes/threads pushing into the same buffer then they must be synchronized externally.
     *
     * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
     *
     * errno error list:
     *  - ENOENT : No buffer with ID id exists.
     *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
     *  - EFAULT : buffer_push or the data address inside it references an inaccessible memory area.
     *  - EINVAL : Request is not valid. Never thrown inside the module, likely you forgot to initialize via raven_init().
     *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
     *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
     *  - EACCES : The buffer isn't owned by the calling process.
     *
     *  Please consult your linux manual to see what errno codes ioctl might produce.
     */
    int push(T data);

    /**
 * Pushes an event to an event buffer in O(1) constant time.
 *
 * Pushes the data to the event buffer via an ioctl call to /dev/raven.
 *
 * The event timestamp is generated from clock monotonic inside the kernel call. Only the buffers owner can push events to a buffer.
 *
 * @param data the data to push.
 *
 * @warning push is not synchronized internally, that means if you want multiple processes/threads pushing into the same buffer then they must be synchronized externally.
 *
 * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
 *
 * errno error list:
 *  - ENOENT : No buffer with ID id exists.
 *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
 *  - EFAULT : buffer_push or the data address inside it references an inaccessible memory area.
 *  - EINVAL : Request is not valid. Never thrown inside the module, likely you forgot to initialize via raven_init().
 *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
 *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
 *  - EACCES : The buffer isn't owned by the calling process.
 *
 *  Please consult your linux manual to see what errno codes ioctl might produce.
 */
    int push_ref(T & data);

    /**
     * Configures this EventWriter to write to a buffer with an id of buffer_id.
     *
     * @param buffer_id the buffer to configure with event writer with.
     */
    void configure(key_t buffer_id);
};

template<class T>
EventWriter<T>::EventWriter()  {
    writer.buffer_id=-1;
}

template<class T>
EventWriter<T>::EventWriter(key_t id) {
    writer.buffer_id=id;
}

template<class T>
int EventWriter<T>::push_ref(T & data) {
    writer.data=&data;
    return raven_push(&writer);
}

template<class T>
int EventWriter<T>::push(T data) {
    push_ref(data);
}

template<class T>
void EventWriter<T>::configure(key_t buffer_id) {
    writer.buffer_id=buffer_id;
}
#endif //KERNEL_EVENTWRITER_H


