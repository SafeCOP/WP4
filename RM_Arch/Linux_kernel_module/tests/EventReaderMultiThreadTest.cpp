#include <string.h>
#include <errno.h>
#include <iostream>
#include "EventReaderMultiThreadTest.h"
#include "../lib/cpp/EventWriter.h"
#include "../lib/cpp/EventReader.h"

CPPUNIT_TEST_SUITE_REGISTRATION(EventReaderMultiThreadTest);


char static compareTimespec(const struct timespec time1, const struct timespec time2) {
    if (time1.tv_sec < time2.tv_sec)
        return -1;
    else if (time1.tv_sec > time2.tv_sec)
        return 1;
    else if (time1.tv_nsec < time2.tv_nsec)
        return -1;
    else if (time1.tv_nsec > time2.tv_nsec)
        return 1;
    else return 0;
}

EventReaderMultiThreadTest::EventReaderMultiThreadTest() : MultiThreadTestTemplate() {
    buffer.id = 0;
    buffer.length = BUFFER_SIZE;
    buffer.index_size = sizeof(StressData<STRESS_DATA_SIZE>);
}

void EventReaderMultiThreadTest::init() {
    int id = raven_init();
    CPPUNIT_ASSERT(id > 0);

    int op = raven_buffer_destroy(buffer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(buffer.id, buffer.length, buffer.index_size);
    CPPUNIT_ASSERT(op >= 0);
}

void EventReaderMultiThreadTest::term() {
    int op = raven_buffer_destroy(buffer.id);
    CPPUNIT_ASSERT(op == 0);
    int test = raven_term();
    CPPUNIT_ASSERT(test == 0);
}

void EventReaderMultiThreadTest::reader_function(MultiThreadTestReaderStats * stats) {
    EventReader<StressData<STRESS_DATA_SIZE> > reader(buffer.id);

    struct timespec last_read_timestamp = {};
    unsigned long long end = NUMBER_OF_EVENTS * (unsigned long long) 5;
    unsigned long long counter = 0;

    Event<StressData<STRESS_DATA_SIZE> > event;
    bool gap;
    bool empty;
    struct timespec next, last;
    clock_gettime(CLOCK_MONOTONIC, &next);
    last = next;
    do {
        struct timespec pop_begin;
        clock_gettime(CLOCK_MONOTONIC, &pop_begin);
        int test= reader.pop(event, empty, gap);
        if(test!=0){
            std::cerr << "raven pop failed at "<<  __FILE__ << " line:" << __LINE__ << " error: " << strerror(errno) << std::endl;
            stats->increment_number_of_errors();
        }
        struct timespec pop_end;
        clock_gettime(CLOCK_MONOTONIC, &pop_end);
        stats->update_op_time(pop_begin, pop_end);

        reader_read_checks(stats, counter, event, last_read_timestamp, empty, gap, pop_begin, pop_end);

        reader_schedule(stats, next, last);
    } while (counter < end);

}

void EventReaderMultiThreadTest::writer_function(MultiThreadTestStats & stats) {
    struct timespec next, last;
    StressData<STRESS_DATA_SIZE> data;
    EventWriter<StressData<STRESS_DATA_SIZE> > writer(buffer.id);
    clock_gettime(CLOCK_MONOTONIC, &next);
    last = next;
    for (unsigned long long i = 0; i < NUMBER_OF_EVENTS; i++) {
        struct timespec push_begin;
        clock_gettime(CLOCK_MONOTONIC, &push_begin);
        int op = writer.push(data);
        struct timespec push_end;
        clock_gettime(CLOCK_MONOTONIC, &push_end);
        if (op != 0) {
            std::cerr << "raven_buffer_push failed at "<<  __FILE__ << " line:" << __LINE__ << " error: " << strerror(errno) << std::endl;
            stats.increment_number_of_errors();
        }
        stats.update_op_time(push_begin, push_end);

        data.increment();
        writer_schedule(&stats, next, last);
    }
}

std::string  EventReaderMultiThreadTest::get_test_name() const{
    return "Event Reader Multithread test";
}
