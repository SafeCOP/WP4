#include <iostream>

#include "SynchronizedEventReaderMultiThreadTest.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SynchronizedEventReaderMultiThreadTest);

MultiThreadTestStats *SynchronizedEventReaderMultiThreadTest::writer_function() {
    MultiThreadTestStats *stats = new MultiThreadTestStats();
    EventWriter<StressData<STRESS_DATA_SIZE> > eventWriter1;
    buf1.configWriter(eventWriter1);

    EventWriter<StressData<STRESS_DATA_SIZE> > eventWriter2;
    buf2.configWriter(eventWriter2);

    StressData<STRESS_DATA_SIZE> data;

    struct timespec next, last;
    clock_gettime(CLOCK_MONOTONIC, &next);
    last = next;
    bool change = false;
    for (unsigned long long i = 0; i < NUMBER_OF_EVENTS; i++) {
        struct timespec push_begin;
        if (change) {
            clock_gettime(CLOCK_MONOTONIC, &push_begin);
            eventWriter1.push(data);
        } else {
            clock_gettime(CLOCK_MONOTONIC, &push_begin);
            eventWriter2.push(data);
        }
        struct timespec push_end;
        clock_gettime(CLOCK_MONOTONIC, &push_end);
        stats->update_op_time(push_begin, push_end);

        data.increment();
        change = !change;

        writer_schedule(stats, next, last);
    }

    return stats;
}

MultiThreadTestReaderStats *SynchronizedEventReaderMultiThreadTest::reader_function() {
    MultiThreadTestReaderStats *stats = new MultiThreadTestReaderStats();
    struct timespec synch = {};

    SynchronizedEventReader<StressData<STRESS_DATA_SIZE> > eventReader1;
    buf1.configSynchronizedEventReader(&synch, eventReader1);

    SynchronizedEventReader<StressData<STRESS_DATA_SIZE> > eventReader2;
    buf2.configSynchronizedEventReader(&synch, eventReader2);

    unsigned long long end = NUMBER_OF_EVENTS * (unsigned long long) 5;
    unsigned long long counter = 0;
    struct timespec last_read_timestamp;
    last_read_timestamp.tv_nsec = 0;
    last_read_timestamp.tv_sec = 0;
    Event<StressData<STRESS_DATA_SIZE> > event;
    bool gap;
    bool change = false;
    bool pop;

    struct timespec next, last;
    clock_gettime(CLOCK_MONOTONIC, &next);
    last = next;
    do {
        struct timespec pop_begin;
        if (change) {
            clock_gettime(CLOCK_MONOTONIC, &pop_begin);
            pop = eventReader1.pop(event, gap);
        } else {
            clock_gettime(CLOCK_MONOTONIC, &pop_begin);
            pop = eventReader2.pop(event, gap);
        }
        struct timespec pop_end;
        clock_gettime(CLOCK_MONOTONIC, &pop_end);

        reader_read_checks(stats, counter, event, last_read_timestamp, pop, gap, pop_begin, pop_end);

        if (pop) change = !change;

        reader_schedule(stats, next, last);
    } while (counter < end);

    return stats;
}

std::string SynchronizedEventReaderMultiThreadTest::get_test_name() const {
    return "Synchronized EventReader Multithread Test";
}
