#ifndef KERNEL_MULTITHREADTESTSTATS_H
#define KERNEL_MULTITHREADTESTSTATS_H

#include "time.h"
#include "../../lib/cpp/timespecUtils.h"

/**
 * This class tracks statistics for MultiThread tests.
 *
 * This class is generally used for writer statistics. A subclass named MultiThreadTestReaderStats generally tracks reader stats.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
class MultiThreadTestStats {
private:
    /**
     * The number of errors.
     */
    unsigned long long number_of_errors;

    /**
     * The number of operations that this tracker has tracked.
     */
    unsigned long long number_of_ops;

    /**
     * The number of deadline misses.
     */
    unsigned long long number_of_deadline_misses;

    /**
    * The smallest time the operation tracked by this event tracker took.
    */
    struct timespec min_op_time;

    /**
    * The biggest time the operation tracked by this event tracker took.
    */
    struct timespec max_op_time;

    /**
    * The smallest loop time the operation tracked by this event tracker took.
    */
    struct timespec min_loop_time;

    /**
    * The biggest loop time the operation tracked by this event tracker took.
    */
    struct timespec max_loop_time;

public:

    /**
   * Instantiates a new MultiThreadTestStats object.
   */
    MultiThreadTestStats();

    /**
     * Gets the number of errors.
     *
     * @return the number of errors.
     */
    unsigned long long int getNumber_of_errors() const;

    /**
    * Gets the number of ops.
    *
    * @return the number of ops.
    */
    unsigned long long int getNumber_of_ops() const;

    /**
    * Gets the number of deadline misses.
    *
    * @return the number of dead line misses.
    */
    unsigned long long int getNumber_of_deadline_misses() const;

    /**
    * Gets the min op time.
    *
    * @returns the min op time.
    */
    const timespec &getMin_op_time() const;

    /**
    * Gets the max op time.
    *
    * @returns the max op time.
    */
    const timespec &getMax_op_time() const;

    /* Gets the min loop op time.
    *
    * @returns the min loop op time.
    */
    const timespec &getMin_loop_time() const;

    /**
    * Gets the max op time.
    *
    * @returns the max op time.
    */
    const timespec &getMax_loop_time() const;

    /**
    * Increments the number of errors by 1.
    */
    void increment_number_of_errors();

    /**
    * Increments the number of errors by num.
    *
    * @param num number of errors to increment by.
    */
    void increment_number_of_errors(unsigned int num);

    /**
    * Increments the number of ops by one.
    */
    void increment_number_of_ops();

    /**
    * Increments the number of deadline misses by one.
    */
    void increment_number_of_deadline_misses();

    /**
    * Updates the operation minimum and maximum time.
    *
    * @param start the time when the operation started.
    * @param end the time when the operation ended.
    */
    void update_op_time(struct timespec &start, struct timespec &end);

    /**
    * Updates the operation loop minimum and maximum time.
    *
    * @param start the time when the loop started.
    * @param end the time when the loop ended.
    */
    void update_loop_time(struct timespec &start, struct timespec &end);
    /**
     * Joins this MultiThreadTestStats with another MultiThreadTestStats, combining their stats.
     */
    void join(const MultiThreadTestStats &stat);
};


#endif //KERNEL_MULTITHREADTESTSTATS_H
