#include "timespecUtils.h"

bool operator<(const struct timespec &time1, const struct timespec &time2) {
    if (time1.tv_sec < time2.tv_sec)
        return true;
    else if (time1.tv_sec > time2.tv_sec)
        return false;
    else return time1.tv_nsec < time2.tv_nsec;
}

bool operator<=(const struct timespec &time1, const struct timespec &time2) {
    if (time1.tv_sec < time2.tv_sec)
        return true;
    else if (time1.tv_sec > time2.tv_sec)
        return false;
    else return time1.tv_nsec <= time2.tv_nsec;
}

bool operator>(const struct timespec &time1, const struct timespec &time2) {
    if (time1.tv_sec > time2.tv_sec)
        return true;
    else if (time1.tv_sec < time2.tv_sec)
        return false;
    else return time1.tv_nsec > time2.tv_nsec;
}

bool operator>=(const struct timespec &time1, const struct timespec &time2) {
    if (time1.tv_sec > time2.tv_sec)
        return true;
    else if (time1.tv_sec < time2.tv_sec)
        return false;
    else return time1.tv_nsec >= time2.tv_nsec;
}

bool operator==(const struct timespec &time1, const struct timespec &time2) {
    if (time1.tv_sec != time2.tv_sec)
        return false;
    else return (time1.tv_nsec == time2.tv_nsec);
}


bool operator!=(const struct timespec &time1, const struct timespec &time2) {
    if (time1.tv_sec != time2.tv_sec)
        return true;
    else return time1.tv_nsec != time2.tv_nsec;
}


void timeAdd(struct timespec &t1, const struct timespec &t2) {
#define BILLION 1000000000
    t1.tv_sec += t2.tv_sec;
    t1.tv_nsec += t2.tv_nsec;
    if (t1.tv_nsec >= BILLION) {
        t1.tv_nsec -= BILLION;
        t1.tv_sec++;
    }
}

void operator+=(struct timespec &t1, const struct timespec &t2) {
#define BILLION 1000000000
    t1.tv_sec += t2.tv_sec;
    t1.tv_nsec += t2.tv_nsec;
    if (t1.tv_nsec >= BILLION) {
        t1.tv_nsec -= BILLION;
        t1.tv_sec++;
    }
}


void timeSubtract(const struct timespec &time1, const struct timespec &time2, struct timespec &result) {
    if ((time2.tv_sec < time1.tv_sec) ||
        ((time2.tv_sec == time1.tv_sec) &&
         (time2.tv_nsec <= time1.tv_nsec))) {        /* TIME1 <= TIME2? */
        result.tv_sec = result.tv_nsec = 0;
    } else {                        /* TIME1 > TIME2 */
        result.tv_sec = time2.tv_sec - time1.tv_sec;
        if (time2.tv_nsec < time1.tv_nsec) {
            result.tv_nsec = time2.tv_nsec + 1000000000L - time1.tv_nsec;
            result.tv_sec--;                /* Borrow a second. */
        } else {
            result.tv_nsec = time2.tv_nsec - time1.tv_nsec;
        }
    }
}


struct timespec operator-(const struct timespec &time1, const struct timespec &time2){
    struct timespec result;
    if ((time2.tv_sec < time1.tv_sec) ||
        ((time2.tv_sec == time1.tv_sec) &&
         (time2.tv_nsec <= time1.tv_nsec))) {        /* TIME1 <= TIME2? */
        result.tv_sec = result.tv_nsec = 0;
    } else {                        /* TIME1 > TIME2 */
        result.tv_sec = time2.tv_sec - time1.tv_sec;
        if (time2.tv_nsec < time1.tv_nsec) {
            result.tv_nsec = time2.tv_nsec + 1000000000L - time1.tv_nsec;
            result.tv_sec--;                /* Borrow a second. */
        } else {
            result.tv_nsec = time2.tv_nsec - time1.tv_nsec;
        }
    }

    return result;
}