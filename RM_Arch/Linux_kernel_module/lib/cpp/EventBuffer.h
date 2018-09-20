#ifndef KERNEL_EVENTBUFFER_H
#define KERNEL_EVENTBUFFER_H


#include <stddef.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <limits.h>
#include <sys/ioctl.h>
#include "EventWriter.h"
#include "EventReader.h"
#include "SynchronizedEventReader.h"

/**
 * EventBuffer wrapper.
 *
 * This class wraps the raven c API to represent an EventBuffer.
 *
 * Buffers should be created by an EventBufferFactory.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
template<class T>
class EventBuffer {
private:
    /**
     * The buffer ID.
     */
    key_t id;

    /**
     * The buffers length.
     */
    unsigned int length;

public:
    /**
    * Instantiates a new EventBuffer.
    *
    * The array timestamp values are zeroed out.
    */
    EventBuffer();

    /**
     * Configures an EventWriter for this EventBuffer.
     *
     * Configures an EventWriter to write to this buffer, only one EventWriter for each Buffer may exist, so a check
     * that a writer has not been configured before is done. A writer is only created if no writer has been created
     * before.
     *
     * @param writer an EventWriter to configure.
     *
     * @return true if the writer was configured.
     *
     * @see EventWriter
     */
     bool configureWriter(EventWriter<T> & writer);

    /**
     * Configures a new EventReader for this buffer.
     *
     * @param reader the EventReader to configure.
     *
     * @see EventReader
     */
     void configureReader(EventReader<T> & reader) const;

    /**
     * Configures a new SynchronizedEventReader with afterTime as a synchronization variable.
     *
     * @param synch the synchronization variable to use with this SynchronizedEventReader. This variable is used to
     * synchronize several SynchronizedEventReaders so that each one pops elements that are newer than afterTime.
     * @param reader the SynchronizedEventReader to configure.
     *
     * @see SynchronizedEventReader
     * @see Monitor
     */
    void configureSynchronizedReader(SynchronizedEventReader<T> & reader, struct timespec * synch) const;

    /**
     * Returns the buffers ID.
     */
    key_t getID();

    /**
     * Gets the EventBuffers length.
     *
     * Get length returns template parameter N, thus it is very efficient as the compiler should inline N.
     *
     * @return the EventBuffer length.
     */
    unsigned int getLength() const;

    /**
     * Returns this buffer index size.
     */
    size_t getIndexSize() const;

    /**
     * Checks if the buffer is empty.
     *
     * @return true if the buffer is empty.
     */
    int empty() const;

    /**
     * Configures this EventBuffer to represent another EventBuffer.
     */
    void configure(const key_t id, const unsigned int length);

    /**
     * Deallocates the buffer, freeing all resources associated with it on the kernel side.
     *
     * Destroys an EventBuffer via an ioctl call to /dev/raven. Destroy is synchronized internally. Only the buffers owner can destroy the buffer.
     *
     *  @return returns 0 on success. On error -1 is returned and errno is set appropriately.
     *
     * errno error list:
     *  - ENOENT : No buffer with ID id exists.
     *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
     *  - EINVAL : Request is not valid. Never thrown inside the module, likely you forgot to initialize via raven_init().
     *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
     *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
     *  - EACCES : The buffer isn't owned by the calling process
     *
     */
    int destroy();
};

template<class T>
EventBuffer<T>::EventBuffer()  { }

template<class T>
int EventBuffer<T>::destroy() {
    int ret= raven_buffer_destroy(id);
    if(ret==0)
        id=-1;
    return ret;
}

template<class T>
bool EventBuffer<T>::configureWriter(EventWriter<T> &writer) {
    writer.configure(id);
}

template<class T>
void EventBuffer<T>::configureReader(EventReader<T> &reader) const {
    reader.configure(id);
}

template<class T>
key_t EventBuffer<T>::getID() {
    return id;
}

template<class T>
unsigned int EventBuffer<T>::getLength() const {
    return length;
}

template<class T>
void EventBuffer<T>::configureSynchronizedReader(SynchronizedEventReader<T> &reader, struct timespec *synch) const {
    reader.configure(id,synch);
}

template<class T>
int EventBuffer<T>::empty() const {
    struct event_buffer_info info;
    int op=raven_buffer_info(id, &info);
    if(op==-1) return -1;

    if(info.last_written.tv_nsec==0 && info.last_written.tv_sec==0)
        return 1;
    else
        return 0;
}

template<class T>
void EventBuffer<T>::configure(const key_t id, const unsigned int length) {
    this->id=id;
    this->length=length;
}


template<class T>
size_t EventBuffer<T>::getIndexSize() const {
    return sizeof(T);
}

#endif //KERNEL_EVENTBUFFER_H
