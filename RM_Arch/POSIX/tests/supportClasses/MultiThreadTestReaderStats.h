#ifndef MONITOR_MULTITHREADREADERTESTSTATS_H
#define MONITOR_MULTITHREADREADERTESTSTATS_H

#include "MultiThreadTestStats.h"
#include <time.h>

/**
 * This class tracks statistics for MultiThread tests readers.
 *
 * Writers are tracked by MultiThreadTestStats.
 */
class MultiThreadTestReaderStats : public MultiThreadTestStats {
private:
    /**
     * The number of gaps.
     */
    unsigned long long number_of_gaps;

    /**
     * The number of false gaps.
     */
    unsigned long long number_of_false_gaps;

    /**
     * The number of empty reads.
     */
    unsigned long long number_of_empty_reads;

    /**
     * The smallest gap read time.
     */
    struct timespec min_gap_time;

    /**
     * The largest gap read time.
     */
    struct timespec max_gap_time;
public:

    /**
     * Instantiates a new MultiThreadTestReaderStats object.
     */
    MultiThreadTestReaderStats();

    /**
     * Gets the number of gaps/
     *
     * @return the number of gaps.
     */
    unsigned long long int getNumber_of_gaps() const;

    /**
     * Get the number of false gaps.
     *
     * @return the number of false gaps.
     */
    unsigned long long int getNumber_of_false_gaps() const;

    /**
     * Get the number of empty reads.
     *
     * @return the number of empty reads.
     */
    unsigned long long int getNumber_of_empty_reads() const;

    /**
     * Gets the smallest gap time.
     *
     * @return the smallest gap time.
     */
    const timespec &getMin_gap_time() const;

    /**
     * Gets the biggest gap time.
     *
     * @return the biggest gap time.
     */
    const timespec &getMax_gap_time() const;

    /**
     * Increments the number of gaps by 1.
     */
    void increment_number_of_gaps();

    /**
     * Increment the number of false gaps by one.
     */
    void increment_number_of_false_gaps();

    /**
     * Increment the number of empty reads by one.
     */
    void increment_number_of_empty_reads();

    /**
     * Updates the read min/max gap time.
     *
     * @param start the time when the read started.
     * @param end the time when the read ended.
     */
    void update_gap_time(const timespec &start, const timespec &end);

    /**
     * Joins this MultiThreadTestStats with another MultiThreadTestStats, combining their stats.
     */
    void join(const MultiThreadTestReaderStats &stat);
};

#endif //MONITOR_MULTITHREADTESTSTATS_H
