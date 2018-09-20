#include "Monitor.h"
#include <errno.h>

Monitor::Monitor(const unsigned long seconds, const unsigned long nanoSeconds)
        : lastReadEvent(), schedule_policy(SCHED_OTHER), priority(0), running(false) {
    period.tv_sec = seconds;
    period.tv_nsec = nanoSeconds;
}

Monitor::Monitor(const struct timespec &time)
        : lastReadEvent(), period(time), schedule_policy(SCHED_OTHER), priority(0), running(false) { }

Monitor::Monitor(const unsigned long seconds, const unsigned long nanoSeconds, int schedule_policy, int priority)
        : lastReadEvent(), schedule_policy(schedule_policy), priority(priority), running(false) {
    period.tv_sec = seconds;
    period.tv_nsec = nanoSeconds;
}

Monitor::Monitor(const struct timespec &time, int schedule_policy, int priority)
        : lastReadEvent(), period(time), schedule_policy(schedule_policy), priority(priority), running(false) { }

int Monitor::enableWithDeadlineMissCheck() {
    dead_line_miss_check = true;
    return enable(pthreadFunctionChecked);
}

void *Monitor::pthreadFunctionChecked(void *ptr) {
    Monitor *monitor = (Monitor *) ptr;
    struct timespec now, next;

    clock_gettime(CLOCK_MONOTONIC, &next);
    for (; ;) {
        clock_gettime(CLOCK_MONOTONIC, &now);
        timeAdd(next, monitor->period);

        if (now > next) {
            monitor->dead_line_miss.store(true, memory_order_relaxed);
            monitor->deadLineMissAction();
        }

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
        monitor->run();
        if (monitor->exit.load(memory_order_relaxed)) {
            monitor->running.store(false, memory_order_relaxed);
            return NULL;
        }
    }
}

void *Monitor::pthreadFunctionUnchecked(void *ptr) {
    Monitor *monitor = (Monitor *) ptr;
    struct timespec next;

    clock_gettime(CLOCK_MONOTONIC, &next);
    for (; ;) {
        timeAdd(next, monitor->period);

        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
        monitor->run();
        if (monitor->exit.load(memory_order_relaxed)) {
            monitor->running.store(false, memory_order_release);
            return NULL;
        }
    }
}

void Monitor::asynch_disable() {
    exit.store(true, memory_order_relaxed);
}

void Monitor::synch_disable() {
    asynch_disable();
    while (running.load(memory_order_relaxed));
}

const timespec &Monitor::getPeriod() {
    return period;
}

bool Monitor::isRunning() {
    return running.load(memory_order_relaxed);
}

char Monitor::resetDeadLine() {
    if (!isRunning() || !dead_line_miss_check) return -1;
    return dead_line_miss.exchange(false, memory_order_relaxed);
}

char Monitor::deadLineMiss() {
    if (!isRunning() || !dead_line_miss_check) return -1;
    return dead_line_miss.load(memory_order_relaxed);
}

int Monitor::enable(void *(*f)(void *)) {
    if (!running.load(memory_order_relaxed)) {
        exit.store(false, memory_order_relaxed);
        dead_line_miss.store(false, memory_order_relaxed);

        pthread_attr_t my_attr;
        int ret = pthread_attr_init(&my_attr);
        if (ret != 0) return ret;

        ret = pthread_attr_setschedpolicy(&my_attr, schedule_policy);
        if (ret != 0) goto error;

        struct sched_param param1;
        param1.sched_priority = priority;
        ret = pthread_attr_setschedparam(&my_attr, &param1);
        if (ret != 0) goto error;

        ret = pthread_attr_setdetachstate(&my_attr, PTHREAD_CREATE_DETACHED);
        if (ret != 0) goto error;

        ret = pthread_create(&thread, &my_attr, f, (void *) this);
        if (ret == 0)
            running.store(true, memory_order_relaxed);

        error:
        pthread_attr_destroy(&my_attr);
        return ret;
    } else {
        errno = EALREADY;
        return -1;
    }
}

int Monitor::enable() {
    dead_line_miss_check = false;
    return enable(Monitor::pthreadFunctionUnchecked);
}

void Monitor::quit() {
    asynch_disable();
}

void Monitor::deadLineMissAction() {
    std::cerr << "Dead line miss for monitor" <<std::endl;
}
