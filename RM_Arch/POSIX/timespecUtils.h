#ifndef MONITOR_TIMESPECUTILS_H
#define MONITOR_TIMESPECUTILS_H

#include <time.h>

/**
 * @file timespecUtils.h
 *
 * This file contains operators function definitions to compare timespecs.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */

/**
 * Checks that timespec time1 is smaller than timespec time2.
 *
 * @param time1 a reference to a timespec struct.
 * @param time2 a reference to a timespec struct.
 *
 * returns whether timespec1 is smaller than timespec2.
 */
bool operator<(const struct timespec &time1, const struct timespec &time2);

/**
 * Checks that timespec time1 is smaller or equal than timespec time2.
 *
 * @param time1 a reference to a timespec struct.
 * @param time2 a reference to a timespec struct.
 *
 * returns whether timespec1 is smaller or equal than timespec2.
 */
bool operator<=(const struct timespec &time1, const struct timespec &time2);

/**
 * Checks that timespec time1 is larger than timespec time2.
 *
 * @param time1 a reference to a timespec struct.
 * @param time2 a reference to a timespec struct.
 *
 * returns whether timespec1 is larger than timespec2.
 */
bool operator>(const struct timespec &time1, const struct timespec &time2);

/**
 * Checks that timespec time1 is larger or equal than timespec time2.
 *
 * @param time1 a reference to a timespec struct.
 * @param time2 a reference to a timespec struct.
 *
 * returns whether timespec1 is larger or equal than timespec2.
 */
bool operator>=(const struct timespec &time1, const struct timespec &time2);

/**
 * Checks that timespec time1 is equal to timespec time2.
 *
 * @param time1 a reference to a timespec struct.
 * @param time2 a reference to a timespec struct.
 *
 * returns whether timespec1 is equal to timespec2.
 */
bool operator==(const struct timespec &time1, const struct timespec &time2);

/**
 * Checks that timespec time1 is diferent than timespec time2.
 *
 * @param time1 a reference to a timespec struct.
 * @param time2 a reference to a timespec struct.
 *
 * returns whether timespec1 is diferent thantimespec2.
 */
bool operator!=(const struct timespec &time1, const struct timespec &time2);

/**
 * Adds timespec t2 to timespec t1.
 *
 * @param t1 a timespec to which t2 will be added.
 * @param t2 a timespec to add to t1.
 */
void timeAdd(struct timespec &t1, const struct timespec &t2);

/**
 * Adds timespec t2 to timespec t1.
 *
 * @param t1 a timespec to which t2 will be added.
 * @param t2 a timespec to add to t1.
 */
void operator+=(struct timespec &t1, const struct timespec &t2);

/**
 * Substracts time2 from time1.
 *
 * @param time1 a timespec to subtract from time2.
 * @param time2 a timespec.
 * @param result time2-time1
 */
void timeSubtract(const struct timespec &time1, const struct timespec &time2, struct timespec &result);

/**
 * Substracts time2 from time1.
 *
 * @param time1 a timespec to subtract from time2.
 * @param time2 a timespec to subtract time1.
 *
 * @return
 */
struct timespec operator-(const struct timespec &time1, const struct timespec &time2);

#endif //MONITOR_TIMESPECUTILS_H
