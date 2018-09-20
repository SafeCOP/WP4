#include "MultiThreadTestStats.h"
#include "../../timespecUtils.h"

MultiThreadTestStats::MultiThreadTestStats() : number_of_errors(0), number_of_ops(0), number_of_deadline_misses(0) {
    max_op_time.tv_nsec = 0;
    max_op_time.tv_sec = 0;
    min_op_time.tv_nsec = 99999999999;
    min_op_time.tv_sec = 9999999999999;
    max_loop_time.tv_sec = 0;
    max_loop_time.tv_nsec = 0;
    min_loop_time.tv_nsec = 9999999999999;
    min_loop_time.tv_sec = 9999999999999;
}

unsigned long long int MultiThreadTestStats::getNumber_of_errors() const {
    return number_of_errors;
}

unsigned long long int MultiThreadTestStats::getNumber_of_ops() const {
    return number_of_ops;
}

unsigned long long int MultiThreadTestStats::getNumber_of_deadline_misses() const {
    return number_of_deadline_misses;
}

const timespec &MultiThreadTestStats::getMin_op_time() const {
    return min_op_time;
}

const timespec &MultiThreadTestStats::getMax_op_time() const {
    return max_op_time;
}

const timespec &MultiThreadTestStats::getMin_loop_time() const {
    return min_loop_time;
}

const timespec &MultiThreadTestStats::getMax_loop_time() const {
    return max_loop_time;
}

void MultiThreadTestStats::increment_number_of_errors() {
    ++number_of_errors;
}

void MultiThreadTestStats::increment_number_of_errors(unsigned int num) {
    number_of_errors += num;
}

void MultiThreadTestStats::increment_number_of_ops() {
    ++number_of_ops;
}

void MultiThreadTestStats::increment_number_of_deadline_misses() {
    ++number_of_deadline_misses;
}

void MultiThreadTestStats::update_op_time(struct timespec &start, struct timespec &end) {
    struct timespec result;
    timeSubtract(start, end, result);

    if (result < min_op_time)
        min_op_time = result;

    if (result > max_op_time)
        max_op_time = result;
}

void MultiThreadTestStats::update_loop_time(struct timespec &start, struct timespec &end) {
    struct timespec result;
    timeSubtract(start, end, result);

    if (result < min_loop_time)
        min_loop_time = result;

    if (result > max_loop_time)
        max_loop_time = result;
}

void MultiThreadTestStats::join(const MultiThreadTestStats &stat) {
    number_of_errors += stat.number_of_errors;
    number_of_ops += stat.number_of_ops;
    number_of_deadline_misses += stat.number_of_deadline_misses;

    if (stat.min_op_time < min_op_time)
        min_op_time = stat.min_op_time;

    if (stat.max_op_time > max_op_time)
        max_op_time = stat.max_op_time;

    if (stat.min_loop_time < min_loop_time)
        min_loop_time = stat.min_loop_time;

    if (stat.max_loop_time > max_loop_time)
        max_loop_time = stat.max_loop_time;
}
