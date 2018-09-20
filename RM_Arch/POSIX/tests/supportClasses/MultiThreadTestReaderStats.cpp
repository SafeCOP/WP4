#include "MultiThreadTestReaderStats.h"
#include "../../timespecUtils.h"

MultiThreadTestReaderStats::MultiThreadTestReaderStats()
        : MultiThreadTestStats(), number_of_gaps(0), number_of_false_gaps(0), number_of_empty_reads(0) {
    min_gap_time.tv_nsec = 999999999999;
    min_gap_time.tv_sec = 9999999999999;
    max_gap_time.tv_nsec = 0;
    max_gap_time.tv_sec = 0;
}


unsigned long long int MultiThreadTestReaderStats::getNumber_of_gaps() const {
    return number_of_gaps;
}

unsigned long long int MultiThreadTestReaderStats::getNumber_of_false_gaps() const {
    return number_of_false_gaps;
}

unsigned long long int MultiThreadTestReaderStats::getNumber_of_empty_reads() const {
    return number_of_empty_reads;
}

const timespec &MultiThreadTestReaderStats::getMin_gap_time() const {
    return min_gap_time;
}

const timespec &MultiThreadTestReaderStats::getMax_gap_time() const {
    return max_gap_time;
}

void MultiThreadTestReaderStats::increment_number_of_gaps() {
    ++number_of_gaps;
}

void MultiThreadTestReaderStats::increment_number_of_false_gaps() {
    ++number_of_false_gaps;
}

void MultiThreadTestReaderStats::increment_number_of_empty_reads() {
    ++number_of_empty_reads;
}

void MultiThreadTestReaderStats::update_gap_time(const timespec &start, const timespec &end) {
    struct timespec result;
    timeSubtract(start, end, result);

    if (result < min_gap_time)
        min_gap_time = result;

    if (result > max_gap_time)
        max_gap_time = result;
}

void MultiThreadTestReaderStats::join(const MultiThreadTestReaderStats &stat) {
    MultiThreadTestStats::join(stat);
    number_of_gaps += stat.number_of_gaps;
    number_of_false_gaps += stat.number_of_false_gaps;
    number_of_empty_reads += stat.number_of_empty_reads;

    if (stat.min_gap_time < min_gap_time)
        min_gap_time = stat.min_gap_time;

    if (stat.max_gap_time > max_gap_time)
        max_gap_time = stat.max_gap_time;
}
