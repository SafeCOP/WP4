#ifndef MONITOR_SYNCHRONIZEDEVENTREADER_H
#define MONITOR_SYNCHRONIZEDEVENTREADER_H

#include <time.h>

#include "types.h"
#include "Event.h"
#include "Buffer.h"
#include "IEventReader.h"
#include "timespecUtils.h"

/**
 * SynchronizedEventReader reads events from an EventBuffer while using a shared timeStamp variable between other
 * SynchronizedEventReaders. Events popped are always larger than this shared timestamp, in FIFO order.
 *
 * SynchronizedEventReader does what it can in situations where events are written faster than it can read them. The reader
 * always finds the first element larger than the shared synchronization variable to read next.
 *
 * SynchronizedEventReaders are instantiated or configured by EventBuffers but are meant to be used exclusively by Monitors. Users
 * can instantiate a SynchronizedEventReader via the Monitor configSynchronizedEventReader method.
 *
 * @see EventBuffer
 * @see Monitor
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
template<typename T>
class SynchronizedEventReader : public IEventReader<T> {
private:
    /**  Constant pointer to a constant circular Buffer this EventReader performs atomic read operations from.
     * @see Buffer
     */
    const Buffer<T> *buffer;

    /**
     * The timestamp of the last read event.
     */
    struct timespec lastReadEvent;

    /** A pointer to the synchronization variable. Events popped from the buffer are always > than this.
     * Not thread safe. */
    struct timespec *afterTime;

    /** Pointer to a constant event which is the current index on this EventReader. */
    length_t index;

    /**
     * Finds the buffer tail, then finds the first element larger than the synchronization variable, pre advancing the index
     * for the next read, and updating the lastReadEvent timestamp and afterTime synchronization variable.
     *
     * When elements we have not read have been overwritten from the buffer that are smaller than the synchronization
     * variable afterTime, there is a gap in the trace and the gap boolean is set.  We have also lost track of the buffer
     * head tail, thus we no longer know where the buffer starts and ends.
     *
     * It becomes necessary to find the buffers tail, where the oldest element is, for example : [3,4,2], in this case 2
     * is the tail where the last element is.
     *
     * To do this we use a while loop to go backwards to find the first element that is larger than the afterTime
     * synchronization variable, that is:  until the next index is larger than the current index or the next
     * index is smaller than the afterRead synchronization variable. We overwrite the return variable event with that index -1,
     * increment to the next index, update the lastReadEvent timestamp and the after time, and finally we set gap to true
     * and return true.
     *
     *   \dot
     *      digraph popGap {
     *          node[shape=doublecircle]; 5;
     *          node[shape=circle]
     *          1->2 [label="Decrement Index"]
     *          2->3 [label="Read element at index"]
     *          3->1 [label="if element < than last read element and > than the synch variable"]
     *          3->4 [label="else"]
     *          4->5 [label="return last read element"]
     *      }
     *   \enddot
     *
     *
     *  Notice that going backwards is safe because:
     *
     * - If a writer writes as fast as we read, going forward could possibly take an infinite amount of time, but going
     * backwards guarantees convergence as each party goes in opposite directions.
     * - Even if a buffer overflows on an overflow, if the next element we read is larger than then one we current read,
     * we have definitively found the smallest element in the EventBuffer.
     *
     * @param tempEvent a reference to a temporary object used for reading operations.
     * @param afterTime a reference to  the afterTime variable which has been de-referenced before.
     * @param lastOverwritten a reference to the lastOverWritten timeStamp in the buffer.
     * @param event a reference to the object which will be used to return the new event.
     * @param gap a reference to the gap boolean responsible for checking gaps, guaranteed to be set to true.
     *
     * @return whether an event was read, guaranteed to be true.
     */
    bool popGap(Event<T> &tempEvent, const struct timespec &afterTime, struct timespec &lastOverwritten,
                Event<T> &event, bool &gap);

    /**
     * Finds the buffers tail, then finds the smallest element that is larger than afterTime. Index, lastReadEvent, afterTime
     * variables are not updated.
     *
     * Functions the same as the popGap method but the state variables are not updated, see popGap for more info.
     *
     * @param tempIndex a reference to a pointer to the temporary index. struct event_buffer eventBufer;
     * @param tempEvent a reference to a temporary object used for reading operations.
     * @param afterTime a reference to  the afterTime variable which has been de-referenced before.
     * @param lastOverwritten a reference to the lastOverWritten timeStamp in the buffer.
     * @param event a reference to the object which will be used to return the new event.
     * @param gap a reference to the gap boolean responsible for checking gaps, guaranteed to be set to true.
     *
     * @return whether an event was read, guaranteed to be true.
     */
    bool peekGap(length_t tempIndex, Event<T> &tempEvent, const struct timespec &afterTime,
                 struct timespec &lastOverwritten, Event<T> &event, bool &gap);

public:

    /**
     * Instantiates a new Synchronized Event Reader.
     *
     * Nothing is initialized.
     */
    SynchronizedEventReader();

    /**
     * Instantiates a new SynchronizedEventReader.
     *
     * Instantiates a new SynchronizedEventReader that reads elements from Buffer buffer that are larger than the synch
     * variable afterTime.
     *
     * @param buffer a reference to a pointer to a Buffer, where elements will be read from.
     * @param afterTime a reference to a pointer to a synchronization variable, events popped from this buffer will be larger
     *  than this variable.
     *
     */
    SynchronizedEventReader(const Buffer<T> *const &buffer, struct timespec *const &afterTime);

    /**
     * Pops the next element in the buffer that is larger than the sync variable.
     *
     * The pop operation pops the next element that is larger than the synch variable into event output parameter,
     * while checking for a gap and taking the necessary measures if there is a gap in the trace.
     *
     * The pop operation consists of the following steps:
     *
     * - First we check that there are elements in the buffer that are larger than the synchronization variable.
     *      -# If there are none, then we can move the index to the buffers tail and set gap=false, finally, we return false.
     *
     * - If there are, we read events until we find a timestamp that is larger than the synchronization variable, while
     *   updating the last read timestamp.
     *
     * - We read the last overwritten timestamp:
     *      -# If it is larger than the last read element, then a gap could have occurred, we call popGap to handle the
     *      problem for us and pop the correct element.
     *      -# If it is not,  we update the SynchronizedReader state variables, lastReadEvent, afterTime,
     *      pre advance the index for the next read , overwrite the data on the output variable event,
     *      set the gap bool, and return true.
     *
     * \dot
     *      digraph pop {
     *          node[shape=doublecircle]; 2 6 9;
     *          node[shape=circle]
     *
     *          1->2[label="If all elements in the Buffer < than the synch variable"]
     *          1->3[label="If there are elements in the buffer that are > than the synch variable"]
     *          3->4[label="Read an Element"]
     *          4->3[label="if timestamp <= than the synchronization timestamp"]
     *          4->5[label="read last overwritten"]
     *          5->6[label="if last overwritten > last_read"]
     *          6->7[label="return popGap"]
     *          5->8[label="If lastOverwritten is <= than the last read timestamp"]
     *          8->9[label="return element"]
     *      }
     * \enddot
     *
     * Please note that reading an index and reading the last_overwritten time stamp is
     * not an atomic operation. We might have read the next sequentially correct element,
     * but because of preemptions, a data race, by the time we read the
     * last_overwritten, it might be > than the last read. Thus a gap warning will be thrown.
     *
     * Caution should be given to the order of operations in the buffer as these are very important, otherwise data
     * races can occur that can leave the EventReader and the read data in an inconsistent state. It is very important
     * to read an element first, and then check the last overwritten timestamp for a possible gap in the buffer.
     *
     * @param event a reference to a Element object which will be updated with the new data.
     * @param gap if there was a gap in the buffer: if one or more elements were overwritten that we didn't read, it is
     * always updated.
     *
     * @return true if an element was read, false if the RoundBuffer is empty.
     *
     * @see popGap
     */
    bool pop(Event<T> &event, bool &gap);

    /**
     * Peeks the next event from the Buffer.
     *
     * The peek operation consists of almost the exact same operations as the pop operation. The only difference is that
     * the index, the lastReadEvent, and the afterTime synchronization variable are not updated.
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
     * PopHead requests the current the current writer index from the buffer, and checks that the event is newer than
     * the synchronization afterTime variable.
     *
     * Pop head also checks for a gap in the trace, that is if the last overwritten element is larger than the afterRead
     * variable.
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
     * PeekHead works much like popHead, except that the buffer state is not updated: the index, the last read element,
     * and the afterTime synchronization variables are not updated.
     *
     * see popHead for more information.
     *
     * @param event a reference to a Element object which will be updated with the new data.
     * @param gap if there was a gap in the buffer: if one or more elements were overwritten that we didn't read, it is
     * always updated.
     *
     * @return true if an element was read.
     */
    bool peekHead(Event<T> &event, bool &gap);

    /**
     * Checks if the Buffer is empty.
     *
     * Checks if the buffer is empty by requesting the buffers head, if the buffers head is smaller or equal to the
     * synchronization variable afterTime, then we are certain the buffer is empty.
     *
     * @return true if the buffer is empty.
     */
    bool empty() const;

    /**
    * Sets this SynchronizedEventReader Buffer.
    *
    * Called during SynchronizedEventReader configuration by the EventBuffer. LastReadEvent is reset to zero, thus the same
    * EventReader can be reconfigured to read from different buffers.
    *
    * @param buffer a constant pointer to a constant buffer to configure this EventReader to.
    */
    void setBuffer(const Buffer<T> *const buffer);

    /**
     * Sets this SynchronizedEventReader synchronized time variable.
     *
     * Called during SynchronizedEventReader configuration by the EventBuffer. LastReadEvent is reset to zero, thus the same
     * EventReader can ben reconfigured to read from multiple buffers.
     *
     * @param afterTime a reference to a constant pointer to a timespec structure that will be used as a synch variable between
     * SynchronizedEventBuffers.
     */
    void setAfterTime(struct timespec *const &afterTime);
};

template<typename T>
SynchronizedEventReader<T>::SynchronizedEventReader() { }

template<typename T>
SynchronizedEventReader<T>::SynchronizedEventReader(const Buffer<T> *const &buffer, struct timespec *const &afterTime)
        : buffer(buffer), lastReadEvent(), afterTime(afterTime), index(1) { }


template<typename T>
bool SynchronizedEventReader<T>::popGap(Event<T> &tempEvent, const struct timespec &afterTime,
                                        struct timespec &lastOverwritten, Event<T> &event, bool &gap) {
    if (index != 0) --index;
    else index = buffer->length - 1;

    Event<T> loopEvent;
    buffer->readIndex(loopEvent, index);
    //find the first element that is larger than the afterTime synchronization variable
    while (loopEvent < tempEvent && loopEvent > afterTime) {
        tempEvent = loopEvent;
        if (index != 0) --index;
        else index = buffer->length - 1;
        buffer->readIndex(loopEvent, index);
    }

    buffer->readLastOverwritten(lastOverwritten);

    lastReadEvent = tempEvent.getTime();
    (*this->afterTime) = lastReadEvent;

    //we increment the index for the next read, we must increment twice as we are pointing to the loopEvent and not tempEvent.
    if (++index >= buffer->length) index = 0;
    if (++index >= buffer->length) index = 0;

    event = tempEvent;
    gap = lastOverwritten > afterTime;
    return true;
}

template<typename T>
bool SynchronizedEventReader<T>::pop(Event<T> &event, bool &gap) {
    const struct timespec afterTime = *this->afterTime;
    struct timespec tempTime;
    length_t tempIndex = buffer->readLastWritten(tempTime);
    //check if there are larger elements in the buffer than the one we last read
    if (tempTime <= afterTime) {
        //there are no new elements in the buffer
        index = tempIndex + 1;
        if (index >= buffer->length) index = 0;
        gap = false;
        lastReadEvent = tempTime;
        return false;
    } else {
        //there are new elements in the buffer
        Event<T> tempEvent;
        struct timespec lastOverwritten;

        buffer->readIndex(tempEvent, index);
        while (tempEvent <= afterTime) {
            lastReadEvent = tempEvent.getTime();
            if (++index >= buffer->length) index = 0;
            buffer->readIndex(tempEvent, index);
        }

        buffer->readLastOverwritten(lastOverwritten);
        if (lastOverwritten > lastReadEvent) //we have lost elements,
            return popGap(tempEvent, afterTime, lastOverwritten, event, gap); //we need to find the buffer tail

        //increment the index for the next read
        if (++index >= buffer->length) index = 0;

        lastReadEvent = tempEvent.getTime();

        *this->afterTime = lastReadEvent;
        gap = false;
        event = tempEvent;
        return true;
    }
}

template<typename T>
bool SynchronizedEventReader<T>::peekGap(length_t tempIndex, Event<T> &tempEvent,
                                         const struct timespec &afterTime, struct timespec &lastOverwritten,
                                         Event<T> &event, bool &gap) {
    if (tempIndex != 0) --tempIndex;
    else tempIndex = buffer->length - 1;
    Event<T> loopEvent;
    buffer->readIndex(loopEvent, tempIndex);
    //find the first element that is larger than the afterTime synchronization variable
    while (loopEvent < tempEvent && loopEvent > afterTime) {
        tempEvent = loopEvent;
        if (tempIndex != 0) --tempIndex;
        else tempIndex = buffer->length - 1;
        buffer->readIndex(loopEvent, tempIndex);
    }

    buffer->readLastOverwritten(lastOverwritten);

    event = tempEvent;
    gap = lastOverwritten > afterTime;
    return true;
}

template<typename T>
bool SynchronizedEventReader<T>::peek(Event<T> &event, bool &gap) {
    const struct timespec afterTime = *this->afterTime;
    struct timespec tempTime;
    length_t tempIndex = buffer->readLastWritten(tempTime);

    if (tempTime <= afterTime) {
        //there are no new elements in the buffer
        //we set the index to the head t avoid reading unnecessary elements
        index = tempIndex + 1;
        if (index >= buffer->length) index = 0;
        gap = false;
        return false;
    } else {
        //there are new elements in the buffer
        tempIndex = index;
        Event<T> tempEvent;
        struct timespec lastOverwritten;

        buffer->readIndex(tempEvent, tempIndex);
        while (tempEvent <= afterTime) {
            if (++tempIndex >= buffer->length) tempIndex = 0;
            buffer->readIndex(tempEvent, tempIndex);
        }

        buffer->readLastOverwritten(lastOverwritten);
        if (lastOverwritten > lastReadEvent)           //we have lost elements,
            return peekGap(tempIndex, tempEvent, afterTime, lastOverwritten, event,
                           gap);//we need to find the buffer tail

        gap = false;
        event = tempEvent;
        return true;
    }
}

template<typename T>
bool SynchronizedEventReader<T>::popHead(Event<T> &event, bool &gap) {
    Event<T> localEvent;
    index = buffer->getHead();
    buffer->readIndex(localEvent, index);
    const struct timespec afterTime = *this->afterTime;
    if (localEvent > afterTime) {
        struct timespec lastOverwritten;
        buffer->readLastOverwritten(lastOverwritten);
        lastReadEvent = localEvent.getTime();
        *this->afterTime = lastReadEvent;
        // we increment the index for the next read
        if (++index > buffer->length) index = 0;
        event = localEvent;
        gap = lastOverwritten > afterTime;
        return true;
    } else {
        lastReadEvent = localEvent.getTime();
        gap = false;
        return false;
    }
}

template<typename T>
bool SynchronizedEventReader<T>::peekHead(Event<T> &event, bool &gap) {
    length_t tempIndex = buffer->getHead();
    Event<T> localEvent;
    struct timespec lastOverwritten;

    buffer->readIndex(localEvent, tempIndex);
    buffer->readLastOverwritten(lastOverwritten);
    const struct timespec afterTime = *this->afterTime;
    if (localEvent > afterTime) {
        //there are new elements in the buffer
        gap = lastOverwritten > afterTime;
        event = localEvent;
        return true;
    } else {
        //there are no new elements in the buffer
        gap = false;
        return false;
    }
}

template<typename T>
bool SynchronizedEventReader<T>::empty() const {
    struct timespec time;
    buffer->readTime(time, buffer->getHead());
    return time <= *afterTime;
}

template<typename T>
void SynchronizedEventReader<T>::setBuffer(const Buffer<T> *const buffer) {
    lastReadEvent.tv_nsec = 0;
    lastReadEvent.tv_sec = 0;
    this->buffer = buffer;
    index = 1;
}

template<typename T>
void SynchronizedEventReader<T>::setAfterTime(struct timespec *const &afterTime) {
    this->afterTime = afterTime;
}

#endif //MONITOR_SYNCHRONIZEDEVENTREADER_H
