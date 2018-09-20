#ifndef KERNEL_EVENTBUFFERINFO_H
#define KERNEL_EVENTBUFFERINFO_H

#include "../../lib/c/raven_api.h"

/**
 * Aggregates information about an EventBuffer.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
class EventBufferInfo {
private:
    /**
     * The buffer info structure used to request information from the raven module.
     */
    struct event_buffer_info info;

public:

    /**
     * Instantiates a new EventBufferInfo.
     */
    EventBufferInfo();

    /**
     * Instantiates a new EventBufferInfo.
     *
     * @param info the info to instantiate this EventBuffer with.
     */
    EventBufferInfo(struct event_buffer_info & info);

    /**
     * Gets the last written timestamp for this buffer.
     *
     * @return the last written timestamp for this buffer.
     */
    const struct timespec & getLastWritten() const;

    /**
     * Gets the timestamp of the last overwritten index.
     *
     * @return  the timestamp of the last overwritten index.
     */
    const struct timespec & getLastOverwritten() const;

    /**
     * Gets the buffer id this information is associated with.
     *
     * @return the buffer id this information is associated with.
     */
    key_t getBufferId() const;

    /**
     * Gets the pid of the buffers owner.
     */
    pid_t getBufferOwner() const;

    /**
     * Gets the length of this buffer.
     */
    unsigned int getLength() const;

    /**
     * Gets the index size of this buffer.
     */
    unsigned short getIndexSize() const;


    /**
     * Gets the EventBufferInfo struct for this object.
     *
     * @return the EventBufferInfo struct for this object.
     */
    struct event_buffer_info & getStructEventBufferInfo();


    /**
     * Updates this object info.
     *
     * @param info the info to update this object with.
     */
    void update(struct event_buffer_info & info);
};

EventBufferInfo::EventBufferInfo() { }

EventBufferInfo::EventBufferInfo(struct event_buffer_info &info): info(info){ }


const struct timespec &EventBufferInfo::getLastWritten() const {
    return info.last_written;
}

const struct timespec &EventBufferInfo::getLastOverwritten() const {
    return info.last_overwritten;
}

key_t EventBufferInfo::getBufferId() const {
    return info.id;
}

pid_t EventBufferInfo::getBufferOwner() const {
    return info.owner;
}

unsigned int EventBufferInfo::getLength() const {
    return info.length;
}

unsigned short EventBufferInfo::getIndexSize() const {
    return info.index_size;
}


struct event_buffer_info &EventBufferInfo::getStructEventBufferInfo() {
    return info;
}

void EventBufferInfo::update(struct event_buffer_info &info) {
    this->info=info;
}

#endif //KERNEL_EVENTBUFFERINFO_H


