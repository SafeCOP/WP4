#ifndef _MONITOR_EVENTREADER_H_
#define _MONITOR_EVENTREADER_H_

#include <time.h>

#include "types.h"
#include "Event.h"
#include "Buffer.h"
#include "IEventReader.h"

/**
 * Reads events from an EventBuffer.
 *
 * Event Reader reads Events from an EventBuffer FIFO fashion. EventReader does what it can when elements
 * are written faster than it can read. When overflows occurs EventReader tries to find the oldest element it has yet to
 * read and notifies the user of the overflow.
 *
 * EventReaders are instantiated or configured by EventBuffers.
 *
 * @see EventBuffer
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
template<typename T>
class EventReader : public IEventReader<T> {
private:
    /**  Constant pointer to a constant circular Buffer this EventReader performs atomic read operations from.
     * @see Buffer
     */
    const Buffer<T> *buffer;

    /** Pointer to a constant event which is the current index on this EventReader. */
    length_t index;

    /** The timestamp of the last event this buffer read.
     * @see time.h
     */
    struct timespec lastReadEvent;

    /**
     * Finds the buffers tail and pops the element at that index, advancing the index and updating the lastReadEvent timestamp.
     *
     * When elements have been overwritten from the buffer that we have not read, there is a gap in the trace and thus,
     * the gap bool is set. We lose track of the buffer head and tail, we no longer know where the buffer starts and ends.
     *
     * It is necessary to find where the buffer tail is,  where the oldest element is in the buffer, for example: [3,4,2],
     * where 2 is the tail.
     *
     * To do this we make use of a while loop that goes backwards until the next index is larger than the current index,
     * or index+1 > index. We overwrite the return variable event with that index+1, update the last read timestamp.
     * and increment to the next index.
     *
     *   \dot
     *      digraph popGap {
     *          node[shape=doublecircle]; 5;
     *          node[shape=circle]
     *          1->2 [label="Decrement Index"]
     *          2->3 [label="Read element at index"]
     *          3->1 [label="if element < than last read element"]
     *          3->4 [label="if element > than last read element"]
     *          4->5 [label="return last read element"]
     *      }
     *   \enddot
     *
     * Notice that going backwards is safe because:
     *
     * - If a writer writes as fast as we read, going forward could possibly take an infinite amount of time, but going
     * backwards guarantees convergence as each party goes in opposite directions.
     * - Even if a buffer overflows on overflows, if the next element we read is larger than then one we current read,
     * we have definitively found the smallest element in the EventBuffer.
     *
     * @param tempEvent a reference to temporary Event object used for event reading operations.
     * @param event a reference to the object which will be used to return the new event.
     * @param gap a reference to the gap boolean responsible for checking gaps, guaranteed to be set to true.
     *
     * @return whether event was read, guaranteed to be true.
     */
    bool popGap(Event<T> &tempEvent, Event<T> &event, bool &gap);

    /**
     * Finds the buffer tail and peeks the element at that index. Index and lastReadEvent object variables are not updated.
     *
     * Functions the same as pop gap but the event reader state is not updated.
     *
     * @param index the last read index.
     * @param tempEvent a reference to temporary Event object used for event reading operations.
     * @param event a reference to the object which will be used to return the new event.
     * @param gap a reference to the gap boolean responsible for checking gaps, guaranteed to be set to true.
     *
     * @return whether event was read, guaranteed to be true.
     *
     * @see popGap
     */
    bool peekGap(length_t index, Event<T> &tempEvent, Event<T> &event, bool &gap) const;

public:
    /**
     * Instantiates a new EventReader.
     *
     * The event reader is blank and should only be configured by calling getEventReader on an EventBuffer.
     */
    EventReader();

    /**
     * Instantiates a new EventReader.
     *
     * Instantiates a new event reader that reads from buffer.
     *
     * @param buffer a constant pointer that points to a constant buffer.
     */
    EventReader(const Buffer<T> *const &buffer);

    /**
     * Pops the next event from the buffer.
     *
     * The pop operation pops the next element in the buffer that is larger than the lastReadEvent, while checking
     * for a gap in the buffer and taking the necessary measures if there is a gap in the trace.
     *
     * The pop operation consists of the following steps:
     *
     *  - Read the current index.
     *   If the timestamp is smaller, we set gap to false and return false, indicating that there is no gap in the trace and that the
     *   event object was not updated, thus no objects were popped.
     *
     *  - If the element's timestamp is larger than the last_read the buffer is not empty and we read the last_overwritten timestamp:
     *
     *      -# If the last overwritten timestamp is larger than the last read event in this buffer, then we know a gap might
     *      have occurred. We find the buffers tail, for details on this operation see popGap method.
     *
     *      -# If it is smaller or equal, then we haven't missed any events. We overwrite events data with the current
     *      index data, save its timestamp as the last_read, and increase index by one for the next read operation.
     *
     *      \dot
     *      digraph pop {
     *          node[shape=doublecircle]; 3 7 9;
     *          node[shape=circle]
     *          1->2 [label="Read the index"]
     *          2->3 [label="If index timestamp <= than last_read"]
     *          2->4 [label="If index > than the last_read"]
     *          4->5 [label="read the last overwritten timestamp"]
     *          5->6 [label="if we have lost elements"]
     *          6->7 [label="call popGap"]
     *          5->8 [label="if we have not lost elements"]
     *          8->9 [label="return read element"]
     *      }
     *      \enddot
     *
     *
     * Please note that reading an index and reading the last_overwritten time stamp is
     * not an atomic operation. We might have read the next sequentially correct element,
     * but because of preemptions or a mere data race, by the time we read the
     * last_overwritten, it might be > than the last read. Thus a gap warning will be thrown.
     *
     *
     *  Caution should be given to the order of operations in the buffer as these are very important, otherwise data
     *  races can occur that can leave the EventReader and the read data in an inconsistent state. It is very important
     *  to read an element first, and then check the last overwritten timestamp for a possible gap in the buffer.
     *
     * @param event a reference to a Element object which will be updated with the new data.
     * @param gap if there was a gap in the buffer: if one or more elements were overwritten that we didn't read, it is
     * always updated.
     *
     * @return true if an element was read, false if the RoundBuffer is empty.
     *
     * @see popGap .
     */
    bool pop(Event<T> &event, bool &gap);

    /**
     * Peeks the next event from the Buffer.
     *
     * The peek operation consists of almost the exact same operations as the pop operation. The only difference is that
     * the index and the last read event are not updated.
     *
     * See pop for more information.
     *
     * @param event a reference to a Element object which will be updated with the new data.
     * @param gap if there was a gap in the buffer: if one or more elements were overwritten that we didn't read, it is
     * always updated.
     *
     * @return true if an element was read, false if the RoundBuffer is empty.
     *
     * @see pop
     */
    bool peek(Event<T> &event, bool &gap);

    /**
     * Pops the newest event from the buffer.
     *
     * PopHead requests the current writer index from the buffer, and checks that the event is newer than the last one we read.
     * PopHead also checks for gap in the trace, that is if the last overwritten element is larger than the one we last read.
     *
     * @param event a reference to a Element object which will be updated with the new data.
     * @param gap if there was a gap in the buffer: if one or more elements were overwritten that we didn't read, it is
     * always updated.
     *
     * @return true if an element was read.
     */
    bool popHead(Event<T> &event, bool &gap);

    /**
     * Peeks the newest event from the buffer.
     *
     * PeekHead works much like popHead, except that the index and the last read element variables are not updated.
     *
     * see popHead for more information.
     *
     * @param event a reference to a Element object which will be updated with the new data.
     * @param gap if there was a gap in the buffer: if one or more elements were overwritten that we didn't read, it is
     * always updated.
     *
     * @return true if an element was read.
     *
     * @see popHead
     */
    bool peekHead(Event<T> &event, bool &gap);

    /**
     * Checks if the Buffer is empty.
     *
     * Checks if the buffer is empty by requesting the buffers head, if the buffers head is smaller or equal to the last
     * element we read in this buffer, then we are certain no new events were pushed.
     *
     * @return true if the buffer is empty.
     */
    bool empty() const;

    /**
     * Sets this EventReader Buffer.
     *
     * Called during EventReader configuration by the EventBuffer. LastReadEvent is reset to zero, thus the same EventReader can be reconfigured
     * to read from different buffers
     *
     * @param buffer a constant pointer to a constant Buffer to configure this EventReader to.
     */
    void setBuffer(const Buffer<T> *const buffer);
};

template<typename T>
EventReader<T>::EventReader() { }

template<typename T>
EventReader<T>::EventReader(const Buffer<T> *const &buffer)
        : buffer(buffer), index(1), lastReadEvent() {
}

template<typename T>
bool EventReader<T>::popGap(Event<T> &tempEvent, Event<T> &event, bool &gap) {
    if (index == 0) index = buffer->length - 1;
    else --index;

    Event<T> loopEvent;
    buffer->readIndex(loopEvent, index);
    //go back until we find the smallest event in the buffer, as we have overflowed this is always the element we are
    // looking for

    while (loopEvent < tempEvent) {
        tempEvent = loopEvent;
        if (index == 0) index = buffer->length - 1;
        else --index;
        buffer->readIndex(loopEvent, index);
    }

    lastReadEvent = tempEvent.getTime();

    //we increment the index for the next read, we must increment twice as we are pointing to the loopEvent and not tempEvent.
    if (++index >= buffer->length) index = 0;
    if (++index >= buffer->length) index = 0;

    event = tempEvent;
    gap = true;
    return true;
}

template<typename T>
bool EventReader<T>::pop(Event<T> &event, bool &gap) {
    struct timespec lastwritten;
    buffer->readLastWritten(lastwritten);

    if (lastwritten > lastReadEvent) {
        Event<T> tempEvent;
        buffer->readIndex(tempEvent,index);
        struct timespec lastOverwritten;
        buffer->readLastOverwritten(lastOverwritten);

        if (lastOverwritten > lastReadEvent) //we have lost elements,
            return popGap(tempEvent, event, gap);    //we need to find the buffer tail

        //increment the index for the next read
        if (++index >= buffer->length) index = 0;

        lastReadEvent = tempEvent.getTime();
        gap = false;
        event = tempEvent;
        return true;
    } else {
        gap = false;
        return false;
    }
}


template<typename T>
bool EventReader<T>::peekGap(length_t tempIndex, Event<T> &tempEvent, Event<T> &event, bool &gap) const {
    if (tempIndex == 0) tempIndex = buffer->length - 1;
    else --tempIndex;

    Event<T> loopEvent;
    buffer->readIndex(loopEvent, tempIndex);
    //go back until we find the smallest event in the buffer, as we have overflowed this is always the element we are
    // looking for
    while (loopEvent < tempEvent) {
        tempEvent = loopEvent;
        if (tempIndex == 0) tempIndex = buffer->length - 1;
        else --tempIndex;
        buffer->readIndex(loopEvent, tempIndex);
    }

    event = tempEvent;
    gap = true;
    return true;
}

template<typename T>
bool EventReader<T>::peek(Event<T> &event, bool &gap) {
    struct timespec lastwritten;
    buffer->readLastWritten(lastwritten);
    if (lastwritten > lastReadEvent) {
        Event<T> tempEvent;
        buffer->readIndex(tempEvent, index);
        struct timespec lastOverwritten;
        buffer->readLastOverwritten(lastOverwritten);
        if (lastOverwritten > lastReadEvent) //we have lost elements,
            return peekGap(index, tempEvent, event, gap);          //we need to find the buffer tail

        gap = false;
        event = tempEvent;
        return true;
    } else {
        gap = false;
        return false;
    }
}

template<typename T>
bool EventReader<T>::popHead(Event<T> &event, bool &gap) {
    Event<T> localEvent;
    index = buffer->getHead();
    buffer->readIndex(localEvent, index);
    if (localEvent > lastReadEvent) {
        struct timespec lastOverwritten;
        buffer->readLastOverwritten(lastOverwritten);
        gap = lastOverwritten > lastReadEvent;
        //there are new elements in the buffer
        lastReadEvent = localEvent.getTime();
        // we increment the index for the next read
        if (++index >= buffer->length) index = 0;
        event = localEvent;
        return true;
    } else {
        //there are no new elements in the buffer
        gap = false;
        return false;
    }
}

template<typename T>
bool EventReader<T>::peekHead(Event<T> &event, bool &gap) {
    Event<T> localEvent;
    length_t tempIndex = buffer->getHead();
    buffer->readIndex(localEvent, tempIndex);
    if (localEvent > lastReadEvent) {
        struct timespec lastOverwritten;
        buffer->readLastOverwritten(lastOverwritten);
        //there are new elements in the buffer
        gap = lastOverwritten > lastReadEvent;
        event = localEvent;
        return true;
    } else {
        //there are no new elements in the buffer
        gap = false;
        return false;
    }
}

template<typename T>
bool EventReader<T>::empty() const {
    struct timespec event;
    buffer->readLastWritten(event);
    return event <= lastReadEvent;
}

template<typename T>
void EventReader<T>::setBuffer(const Buffer<T> *const buffer) {
    lastReadEvent.tv_nsec = 0;
    lastReadEvent.tv_sec = 0;
    this->buffer = buffer;
    index = 1;
}

#endif //_MONITOR_EVENTREADER_H_
