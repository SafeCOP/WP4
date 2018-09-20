//
// Created by silverone on 8/24/15.
//

#ifndef KERNEL_IEVENTREADER_H
#define KERNEL_IEVENTREADER_H

#include <sys/types.h>
#include "../c/raven_api.h"
#include "Event.h"

/**
 * IEventReader virtual class contains common operations to EventReaders and SynchronizedEventReaders.
 *
 * the raven architecture doesn't distinguish synchronized buffers from regular buffers. Thus the method calls,
 * ioctl codes, and ioctl parameters are exactly the same. It makes sense to aggregate this in a superclass.
 *
 * @see SynchronizedEventReader
 * @see EventReader
 */
template<class T>
class IEventReader {
protected:
    /**
     * The event reader structure.
     */
    struct event_reader reader;

    /**
     * Instantiates a new IEventReader.
     */
    IEventReader();

    /**
     * Instantiates a new IEventReader that reads from the buffer which has the specified id.
     * @param buffer_id the id of the buffers to read events from.
     */
    IEventReader(const key_t buffer_id);

    /**
     * Instantiates a new IEventReader that reads from the buffer which has the specified id, using the specified synchronization variable.
     *
     * @param buffer_id the id of the buffers to read events from.
     * @param synch the synchronizoation variable.
    */
    IEventReader(const key_t buffer_id, const struct timespec * synch);

public:

    /**
     * Pops an event from an event buffer.
     *
     * Pops an event from the buffer via an ioctl call to /dev/raven.
     *
     * @param data An event object where the popped data and timestamp will be stored.
     * @param empty an empty flag which will be set to true if the buffer is empty or false if it is not.
     * @param gap a gap flag which will be set to true if elements that we did not read have been overwritten.
     *
     * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
     *
     * errno error list:
     *  - ENOENT : No buffer with ID id exists.
     *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
     *  - EFAULT : buffer_pop or some address inside it references an inaccessible memory area.
     *  - EINVAL : Request is not valid. This could occur if the index is larger than the buffer event_buffers_length. It is also possible you forgot to initialize via raven_init().
     *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
     *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
     *
     *  Please consult your linux manual to see what errno codes ioctl might produce.
     *
     */
    int pop(Event<T> & data, bool & empty, bool & gap);

    /**
     * Peeks, reads an event from an event buffer without marking it as read.
     *
     * Reads an event from the buffer without marking it as read via an ioctl call to /dev/raven.
     *
     * @param data An event object where the popped data and timestamp will be stored.
     * @param empty an empty flag which will be set to true if the buffer is empty or false if it is not.
     * @param gap a gap flag which will be set to true if elements that we did not read have been overwritten.
     *
     * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
     *
     * errno error list:
     *  - ENOENT : No buffer with ID id exists.
     *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
     *  - EFAULT : buffer_pop or some address inside it references an inaccessible memory area.
     *  - EINVAL : Request is not valid. This could occur if the index is larger than the buffer event_buffers_length. It is also possible you forgot to initialize via raven_init().
     *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
     *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
     *
     *  Please consult your linux manual to see what errno codes ioctl might produce.
     *
     */
    int peek(Event<T> & data, bool & empty, bool & gap);


    /**
     * Pops the newest event from an event buffer.
     *
     * Pops the newest event event from the buffer via an ioctl call to /dev/raven.
     *
     * @param data An event object where the popped data and timestamp will be stored.
     * @param empty an empty flag which will be set to true if the buffer is empty or false if it is not.
     * @param gap a gap flag which will be set to true if elements that we did not read have been overwritten.
     *
     * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
     *
     * errno error list:
     *  - ENOENT : No buffer with ID id exists.
     *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
     *  - EFAULT : buffer_pop or some address inside it references an inaccessible memory area.
     *  - EINVAL : Request is not valid. This could occur if the index is larger than the buffer event_buffers_length. It is also possible you forgot to initialize via raven_init().
     *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
     *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
     *
     *  Please consult your linux manual to see what errno codes ioctl might produce.
     */
    int popHead(Event<T> & data, bool & empty, bool & gap);

    /**
     * Peeks, reads the newest event from an event buffer without marking it as read.
     *
     * Reads the newest event without marking it as read, via an ioctl call to /dev/raven.
     *
     * @param data An event object where the popped data and timestamp will be stored.
     * @param empty an empty flag which will be set to true if the buffer is empty or false if it is not.
     * @param gap a gap flag which will be set to true if elements that we did not read have been overwritten.
     *
     * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
     *
     * errno error list:
     *  - ENOENT : No buffer with ID id exists.
     *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
     *  - EFAULT : buffer_pop or some address inside it references an inaccessible memory area.
     *  - EINVAL : Request is not valid. This could occur if the index is larger than the buffer event_buffers_length. It is also possible you forgot to initialize via raven_init().
     *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
     *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
     *
     *  Please consult your linux manual to see what errno codes ioctl might produce.
     */
    int peekHead(Event<T> & data, bool & empty, bool & gap);

    /**
     * Pops an event from an event buffer.
     *
     * Pops an event from the buffer via an ioctl call to /dev/raven.
     * You can read the data timestamp by calling getEventTime().
     *
     * @param data An event object where the popped data will be stored.
     * @param empty an empty flag which will be set to true if the buffer is empty or false if it is not.
     * @param gap a gap flag which will be set to true if elements that we did not read have been overwritten.
     *
     * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
     *
     * errno error list:
     *  - ENOENT : No buffer with ID id exists.
     *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
     *  - EFAULT : buffer_pop or some address inside it references an inaccessible memory area.
     *  - EINVAL : Request is not valid. This could occur if the index is larger than the buffer event_buffers_length. It is also possible you forgot to initialize via raven_init().
     *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
     *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
     *
     *  Please consult your linux manual to see what errno codes ioctl might produce.
     */
    int pop(T & data, bool & empty, bool & gap);

    /**
     * Peeks, reads an event from an event buffer without marking it as read.
     *
     * Reads an event from the buffer without marking it as read via an ioctl call to /dev/raven.
     *
     * @param data An event object where the popped data will be stored.
     * @param empty an empty flag which will be set to true if the buffer is empty or false if it is not.
     * @param gap a gap flag which will be set to true if elements that we did not read have been overwritten.
     *
     * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
     *
     * errno error list:
     *  - ENOENT : No buffer with ID id exists.
     *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
     *  - EFAULT : buffer_pop or some address inside it references an inaccessible memory area.
     *  - EINVAL : Request is not valid. This could occur if the index is larger than the buffer event_buffers_length. It is also possible you forgot to initialize via raven_init().
     *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
     *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
     *
     *  Please consult your linux manual to see what errno codes ioctl might produce.
     *
     */
    int peek(T & data, bool & empty, bool & gap);

    /**
     * Pops the newest event from an event buffer.
     *
     * Pops the newest event event from the buffer via an ioctl call to /dev/raven.
     *
     * @param data An event object where the popped data will be stored.
     * @param empty an empty flag which will be set to true if the buffer is empty or false if it is not.
     * @param gap a gap flag which will be set to true if elements that we did not read have been overwritten.
     *
     * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
     *
     * errno error list:
     *  - ENOENT : No buffer with ID id exists.
     *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
     *  - EFAULT : buffer_pop or some address inside it references an inaccessible memory area.
     *  - EINVAL : Request is not valid. This could occur if the index is larger than the buffer event_buffers_length. It is also possible you forgot to initialize via raven_init().
     *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
     *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
     *
     *  Please consult your linux manual to see what errno codes ioctl might produce.
     */
    int popHead(T & data, bool & empty, bool & gap);

    /**
     * Peeks, reads the newest event from an event buffer without marking it as read.
     *
     * Reads the newest event without marking it as read, via an ioctl call to /dev/raven.
     *
     * @param data An event object where the popped data will be stored.
     * @param empty an empty flag which will be set to true if the buffer is empty or false if it is not.
     * @param gap a gap flag which will be set to true if elements that we did not read have been overwritten.
     *
     * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
     *
     * errno error list:
     *  - ENOENT : No buffer with ID id exists.
     *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
     *  - EFAULT : buffer_pop or some address inside it references an inaccessible memory area.
     *  - EINVAL : Request is not valid. This could occur if the index is larger than the buffer event_buffers_length. It is also possible you forgot to initialize via raven_init().
     *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
     *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
     *
     *  Please consult your linux manual to see what errno codes ioctl might produce.
     */
    int peekHead(T & data, bool & empty, bool & gap);

    /**
     * Gets the current event time.
     *
     * @return the current event time.
     */
    const struct timespec & getEventTime();

    /**
     * Gets the last read event time.
     *
     * @return the last read event time.
     */
    const struct timespec & getLastReadEvent();

    /**
     * Checks if the buffer is empty.
     *
     * Calls raven buffer info to get the timestamp of the last written element. It then compares it to the last read
     * timestamp or the synchronization variable to check if it is newer.
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
    virtual int emptyBuffer() const=0;
};

template<class T>
IEventReader<T>::IEventReader() { }

template<class T>
IEventReader<T>::IEventReader(key_t buffer_id) {
    raven_init_event_reader(&reader,buffer_id,NULL,NULL);
}

template<class T>
IEventReader<T>::IEventReader(const key_t buffer_id, const struct timespec * synch) {
    raven_init_event_reader(&reader,buffer_id,NULL,synch);
}

template<class T>
int IEventReader<T>::pop(Event<T> & data, bool & empty, bool & gap){
    reader.event_data=&data.getData();
    int ret= raven_pop(&reader);
    if(ret==0){
        empty=reader.empty;
        gap=reader.gap;
        data.setTime(reader.event_time);
    }
    return ret;
}

template<class T>
int IEventReader<T>::pop(T & data, bool & empty, bool & gap){
    reader.event_data=&data;
    int ret= raven_pop(&reader);
    if(ret==0){
        empty=reader.empty;
        gap=reader.gap;
    }
    return ret;
}

template<class T>
int IEventReader<T>::peek(Event<T> & data, bool & empty, bool & gap){
    reader.event_data=&data.getData();
    int ret= raven_peek(&reader);
    if(ret==0){
        empty=reader.empty;
        gap=reader.gap;
        data.setTime(reader.event_time);
    }
    return ret;
}

template<class T>
int IEventReader<T>::peek(T & data, bool & empty, bool & gap){
    reader.event_data=&data;
    int ret = raven_peek(&reader);
    if(ret==0){
        empty=reader.empty;
        gap=reader.gap;
    }
    return ret;
}

template<class T>
int IEventReader<T>::peekHead(Event<T> & data, bool & empty, bool & gap){
    reader.event_data=&data.getData();
    int ret= raven_peek_head(&reader);
    if(ret==0){
        empty=reader.empty;
        gap=reader.gap;
        data.setTime(reader.event_time);
    }
    return ret;
}

template<class T>
int IEventReader<T>::peekHead(T & data, bool & empty, bool & gap){
    reader.event_data=&data;
    int ret= raven_peek_head(&reader);
    if(ret==0){
        empty=reader.empty;
        gap=reader.gap;
    }
    return ret;
}

template<class T>
int IEventReader<T>::popHead(Event<T> & data, bool & empty, bool & gap){
    reader.event_data=&data.getData();
    int ret= raven_pop_head(&reader);
    if(ret==0){
        empty=reader.empty;
        gap=reader.gap;
        data.setTime(reader.event_time);
    }
    return ret;
}

template<class T>
int IEventReader<T>::popHead(T & data, bool & empty, bool & gap){
    reader.event_data=&data;
    int ret= raven_pop_head(&reader);
    if(ret==0){
        empty=reader.empty;
        gap=reader.gap;
    }
    return ret;
}

template<class T>
const struct timespec & IEventReader<T>::getEventTime() {
    return reader.event_time;
}

template<class T>
const struct timespec & IEventReader<T>::getLastReadEvent() {
    return reader.last_read;
}

#endif //KERNEL_IEVENTREADER_H

