#include "EventReaderMultiThreadTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(EventReaderMultiThreadTest);

MultiThreadTestStats *EventReaderMultiThreadTest::writer_function() {
    MultiThreadTestStats *stats = new MultiThreadTestStats();
    EventWriter<StressData<STRESS_DATA_SIZE> > eventWriter;
    buf.configWriter(eventWriter);

    StressData<STRESS_DATA_SIZE> data;
    struct timespec next, last;
    clock_gettime(CLOCK_MONOTONIC, &next);
    last = next;
    for (unsigned long long i = 0; i < NUMBER_OF_EVENTS; i++) {
        struct timespec push_begin;
        clock_gettime(CLOCK_MONOTONIC, &push_begin);
        eventWriter.push(data);
        struct timespec push_end;
        clock_gettime(CLOCK_MONOTONIC, &push_end);
        stats->update_op_time(push_begin, push_end);

        data.increment();
        writer_schedule(stats, next, last);
    }

    return stats;
}


MultiThreadTestReaderStats *EventReaderMultiThreadTest::reader_function() {
    MultiThreadTestReaderStats *stats = new MultiThreadTestReaderStats();
    EventReader<StressData<STRESS_DATA_SIZE> > eventReader;
    buf.configReader(eventReader);

    unsigned long long end = NUMBER_OF_EVENTS * (unsigned long long) 5;
    unsigned long long counter = 0;
    struct timespec last_read_timestamp = {};
    Event<StressData<STRESS_DATA_SIZE> > event;
    bool gap;
    bool pop;

    struct timespec next, last;
    clock_gettime(CLOCK_MONOTONIC, &next);
    last = next;
    do {
        struct timespec pop_begin;
        clock_gettime(CLOCK_MONOTONIC, &pop_begin);
        pop = eventReader.pop(event, gap);
        struct timespec pop_end;
        clock_gettime(CLOCK_MONOTONIC, &pop_end);
        stats->update_op_time(pop_begin, pop_end);

        reader_read_checks(stats, counter, event, last_read_timestamp, pop, gap, pop_begin, pop_end);

        reader_schedule(stats, next, last);
    } while (counter < end);

    return stats;
}

std::string EventReaderMultiThreadTest::get_test_name() const {
    return "Event Reader Multithread test";
}

