#ifndef KERNEL_EVENT_H
#define KERNEL_EVENT_H

#include <time.h>

/**
 * Data container that holds data of template type and keeps a timestamp of the event.
 *
 * Timestamps are type timespec as defined in time.h, timespec is precise to the nanosecond.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
template<typename T>
class Event {
private:
    /** Data container for this Event. */
    T data;
    /** Creation time for this Event.
     * @see time.h
     */
    struct timespec time;

public:
    /**
     * Instantiates a new event.
     *
     * Instantiates a new event, nothing is zeroed out.
     */
    Event();

    /**
     * Instantiates a new event with data as this event.
     *
     * data is instantiated by a copy constructor,
     * time is instantiated by POSIX clock_gettime using CLOCK_REALTIME,
     *
     * @param data a constant reference to the data.
     *
     * @see time.h
     */
    Event(const T &data);

    /**
     * Instantiates a new event with data and time parameters.
     *
     * Both  data and  time are instantiated using copy constructors.
     *
     * @param data a constant reference to the data being copied.
     * @param time constant reference to the time being copied.
     */
    Event(const T &data, const struct timespec &time);


    /**
     * Gets the event current data.
     *
     * @return a reference to this events Data.
     */
    T &getData();

    /**
    * Gets the event current data.
    *
    * @return a reference to this events Data.
    */
    const T &getData() const;

    /**
     * Gets the event creation time.
     *
     * @return the event current time.
     */
    struct timespec &getTime();

    /**
    * Gets the event creation time.
    *
    * @return the event current time.
    */
    const struct timespec &getTime() const;

    /**
     * Sets this event time.
     *
     * @param time the time to set this even to.
     */
    void setTime(const struct timespec & time);

    /**
     * Sets this event data.
     *
     * @param data the data to set on this event.
     */
    void setData(const T & data);

    /**
     * Checks if this object is null.
     *
     * An object is determined to be null if the current time is zeroed out,
     * that is if time.tv_sec is 0 and time_.tv_nsec is 0.
     *
     * @return whether this is a nullObject.
     */
    bool isNull() const;

    /**
     * Copies the Events data pointed by event to this object.
     *
     * @param event The volatile data to copy to this object.
     */
    Event<T> &operator=(const Event<T> *event);

    /**
     * Checks if this event is < than Event  event.
     *
     * @param event Another event.
     * @return whether this event is < than Event  event.
     */
    bool operator<(const Event &event) const;

    /**
    * Checks if this event is <= than Event  event.
    *
    * @param event Another event.
    * @return whether this event is <= than Event  event.
    */
    bool operator<=(const Event &event) const;

    /**
    * Checks if this event is > than Event  event.
    *
    * @param event Another event.
    * @return whether this event is > than Event  event.
    */
    bool operator>(const Event &event) const;

    /**
    * Checks if this event is >= than Event  event.
    *
    * @param event Another event.
    * @return whether this event is >= than Event  event.
    */
    bool operator>=(const Event &event) const;

    /**
    * Checks if this event is == to Event  event.
    *
    * @param event Another event.
    * @return whether this event is == to Event  event.
    */
    bool operator==(const Event &event) const;

    /**
    * Checks if this event is != than Event  event.
    *
    * @param event Another event.
    * @return whether this event is != than Event  event.
    */
    bool operator!=(const Event &event) const;

    /**
    * Checks if this event is < than timespec  time.
    *
    * @param time a timespec as defined by time.h
    * @return whether this event is < than timespec  time.
    */
    bool operator<(const struct timespec &time) const;

    /**
    * Checks if this event is <= than timespec  time.
    *
    * @param time a timespec as defined by time.h
    * @return whether this event is <= than timespec  time.
    */
    bool operator<=(const struct timespec &time) const;

    /**
    * Checks if this event is > than timespec  time.
    *
    * @param time a timespec as defined by time.h
    * @return whether this event is > than timespec  time.
    */
    bool operator>(const struct timespec &time) const;

    /**
    * Checks if this event is >= than timespec  time.
    *
    * @param time a timespec as defined by time.h
    * @return whether this event is >= than timespec  time.
    */
    bool operator>=(const struct timespec &time) const;

    /**
    * Checks if this event is == to timespec  time.
    *
    * @param time a timespec as defined by time.h
    * @return whether this event is == to timespec  time.
    */
    bool operator==(const struct timespec &time) const;

    /**
    * Checks if this event is != than timespec  time.
    *
    * @param time a timespec as defined by time.h
    * @return whether this event is != than timespec  time.
    */
    bool operator!=(const struct timespec &time) const;

    /**
    * Checks if timespec time is < than Event  event.
    *
    * @param time a timespec as defined by time.h
    * @param event an event
    *
    * @return whether timespec  time is < than Event  event.
    */
    template<typename D>
    friend bool operator<(const struct timespec &time, const Event<D> &event);

    /**
    * Checks if timespec time is < than Event  event.
    *
    * @param time a timespec as defined by time.h
    * @param event an event
    *
    * @return whether timespec  time is < than Event  event.
    */
    template<typename D>
    friend bool operator<=(const struct timespec &time, const Event<D> &event);

    /**
    * Checks if timespec  time is <= than Event  event.
    *
    * @param time a timespec as defined by time.h
    * @param event an event
    *
    * @return whether timespec  time is <= than Event  event.
    */
    template<typename D>
    friend bool operator>(const struct timespec &time, const Event<D> &event);

    /**
    * Checks if timespec  time is > than Event  event.
    *
    * @param time a timespec as defined by time.h
    * @param event an event
    *
    * @return whether timespec  time is > than Event  event.
    */
    template<typename D>
    friend bool operator>=(const struct timespec &time, const Event<D> &event);

    /**
    * Checks if timespec  time is == than Event  event.
    *
    * @param time a timespec as defined by time.h
    * @param event an event
    *
    * @return whether timespec  time is == than Event  event.
    */
    template<typename D>
    friend bool operator==(const struct timespec &time, const Event<D> &event);

    /**
    * Checks if timespec  time is != than Event  event.
    *
    * @param time a timespec as defined by time.h
    * @param event an event
    *
    * @return whether timespec  time is != than Event  event.
    */
    template<typename D>
    friend bool operator!=(const struct timespec &time, const Event<D> &event);
};

template<typename T>
Event<T>::Event() { }

template<typename T>
Event<T>::Event(const T &data) : data(data) {
    clock_gettime(CLOCK_MONOTONIC, &time);
}

template<typename T>
Event<T>::Event(const T &data, const struct timespec &time) : data(data), time(time) { }

template<typename T>
T &Event<T>::getData() {
    return data;
}

template<typename T>
const T &Event<T>::getData() const {
    return data;
}

template<typename T>
struct timespec &Event<T>::getTime() {
    return time;
}

template<typename T>
const struct timespec &Event<T>::getTime() const {
    return time;
}

template<class T>
void Event<T>::setTime(const struct timespec & time){
    this->time=time;
}

template<class T>
void Event<T>::setData(const T & data){
    this->data=data;
}

template<typename T>
bool Event<T>::isNull() const {
    return time.tv_nsec == 0 && time.tv_sec == 0;
}

template<typename T>
Event<T> &Event<T>::operator=(const Event *event) {
    data = event->data;
    time = event->time;
    return *this;
}

template<typename T>
bool Event<T>::operator<(Event const &event) const {
    return time < event.time;
}

template<typename T>
bool Event<T>::operator<=(Event const &event) const {
    return time <= event.time;
}

template<typename T>
bool Event<T>::operator>(Event const &event) const {
    return time > event.time;
}

template<typename T>
bool Event<T>::operator>=(Event const &event) const {
    return time >= event.time;
}

template<typename T>
bool Event<T>::operator==(Event<T> const &event) const {
    return time == event.time && data == event.data;
}

template<typename T>
bool Event<T>::operator!=(Event<T> const &event) const {
    return time != event.time;
}

template<typename T>
bool Event<T>::operator<(const struct timespec &time) const {
    return this->time < time;
}

template<typename T>
bool Event<T>::operator<=(const struct timespec &time) const {
    return this->time <= time;
}

template<typename T>
bool Event<T>::operator>(const struct timespec &time) const {
    return this->time > time;
}

template<typename T>
bool Event<T>::operator>=(const struct timespec &time) const {
    return this->time >= time;
}

template<typename T>
bool Event<T>::operator==(const struct timespec &time) const {
    return this->time == time;
}

template<typename T>
bool Event<T>::operator!=(const struct timespec &time) const {
    return this->time != time;
}

template<typename T>
bool operator<(const struct timespec &time, const Event<T> &event) {
    return time < event.time;
}

template<typename T>
bool operator<=(const struct timespec &time, const Event<T> &event) {
    return time <= event.time;
}

template<typename T>
bool operator>(const struct timespec &time, const Event<T> &event) {
    return time > event.time;
}

template<typename T>
bool operator>=(const struct timespec &time, const Event<T> &event) {
    return time >= event.time;
}

template<typename T>
bool operator==(const struct timespec &time, const Event<T> &event) {
    return time == event.time;
}

template<typename T>
bool operator!=(const struct timespec &time, const Event<T> &event) {
    return time != event.time;
}


#endif //KERNEL_EVENT_H
