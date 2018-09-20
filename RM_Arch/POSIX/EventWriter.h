#ifndef _MONITOR_EVENTWRITER_H_
#define _MONITOR_EVENTWRITER_H_

#include "Event.h"
#include "Buffer.h"

/**
 * Writes events to an EventBuffer.
 *
 * Writing operations take O(1) time.
 *
 * EventWriters are instantiated by EventBuffers.
 *
 * @see EventBuffer
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
template<typename T>
class EventWriter {
private:
    /**  Pointer to a circular Buffer this EventWriter writes to.
     * @see Buffer
     */
    Buffer<T> *buffer;

    //notImplemented
    EventWriter(const EventWriter<T> &eventWriter);

    EventWriter<T> &operator=(const EventWriter<T> &eventWriter);

public:
    /**
     * Instantiates a new EventWriter.
     *
     * The event writer is blank and should only be configured by calling configWriter on an EventBuffer.
     */
    EventWriter();

    /**
     * Instantiates a new EventWriter.
     *
     * @param buffer the Buffer to write to.
     *
     * Instantiates a new EventWriter to write to Buffer buffer.
     */
    EventWriter(Buffer<T> *buffer);

    /**
     * Pushes an event to the Buffer.
     *
     * Pushes events to the buffer in O(1) time.
     *
     * @param event a constant reference to the event to be pushed.
     */
    void push(const Event<T> &event);

    /**
    * Pushes an event to the Buffer.
    *
    * Pushes events to the buffer in O(1) time.
    *
    * @param data a constant reference to the data to be pushed.
    */
    void push(const T &data);

    /**
     * Checks whether the EventBuffer this EventWriter writes to is empty.
     *
     * @return true if the EventBuffer this EventWriter writes to is empty.
     */
    bool empty() const;

    /**
    * Sets this EventWriter Buffer.
    *
    * Called during EventWriter configuration by the EventBuffer.
    *
    * @param buffer the Buffer to configure this EventReader to.
    */
    void setBuffer(Buffer<T> *buffer);
};

template<typename T>
EventWriter<T>::EventWriter() : buffer(NULL) { }

template<typename T>
EventWriter<T>::EventWriter(Buffer<T> *buffer) : buffer(buffer) { }

template<typename T>
void EventWriter<T>::push(const Event<T> &event) {
    buffer->push(event);
}

template<typename T>
void EventWriter<T>::push(const T &data) {
    buffer->push(data);
}

template<typename T>
bool EventWriter<T>::empty() const {
    return buffer->empty();
}

template<typename T>
void EventWriter<T>::setBuffer(Buffer<T> *buffer) {
    this->buffer = buffer;
}

#endif //_MONITOR_EVENTWRITER_H_
