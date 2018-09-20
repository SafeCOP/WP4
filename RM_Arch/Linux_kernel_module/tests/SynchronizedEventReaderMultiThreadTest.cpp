#include <string.h>
#include <errno.h>
#include <iostream>

#include "SynchronizedEventReaderMultiThreadTest.h"
#include "../lib/cpp/SynchronizedEventReader.h"
#include "../lib/cpp/EventWriter.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SynchronizedEventReaderMultiThreadTest);

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

SynchronizedEventReaderMultiThreadTest::SynchronizedEventReaderMultiThreadTest() : MultiThreadTestTemplate() {
    buffer1.id = 0;
    buffer1.length = BUFFER_SIZE;
    buffer1.index_size = sizeof(StressData<STRESS_DATA_SIZE>);

    buffer2.id = 1;
    buffer2.length = BUFFER_SIZE;
    buffer2.index_size = sizeof(StressData<STRESS_DATA_SIZE>);
}


void SynchronizedEventReaderMultiThreadTest::init() {
    int id = raven_init();
    CPPUNIT_ASSERT(id > 0);

    int op = raven_acquire(buffer1.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_destroy(buffer1.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(buffer1.id, buffer1.length, buffer1.index_size);
    CPPUNIT_ASSERT(op >= 0);

    op = raven_acquire(buffer2.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_destroy(buffer2.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(buffer2.id, buffer2.length, buffer2.index_size);
    CPPUNIT_ASSERT(op >= 0);
}

void SynchronizedEventReaderMultiThreadTest::term() {
    int op = raven_buffer_destroy(buffer1.id);
    CPPUNIT_ASSERT(op == 0);
    op = raven_buffer_destroy(buffer2.id);
    CPPUNIT_ASSERT(op == 0);
    raven_term();
}


void SynchronizedEventReaderMultiThreadTest::reader_function(MultiThreadTestReaderStats * stats) {
    struct timespec synch={};
    SynchronizedEventReader<StressData<STRESS_DATA_SIZE> > eventReader1;
    eventReader1.configure(buffer1.id,&synch);
    SynchronizedEventReader<StressData<STRESS_DATA_SIZE> > eventReader2;
    eventReader2.configure(buffer2.id,&synch);

    struct timespec last_read_timestamp = {};
    unsigned long long end = NUMBER_OF_EVENTS * (unsigned long long) 5;
    unsigned long long counter = 0;
    bool change = true;

    StressData<STRESS_DATA_SIZE> data;
    struct event_reader bpop[2];
    raven_init_event_reader(&bpop[0], buffer1.id, &data, &synch);
    raven_init_event_reader(&bpop[1], buffer2.id, &data, &synch);
    Event<StressData<STRESS_DATA_SIZE> > event;

    bool gap;
    bool empty;
    int test;

    struct timespec next, last;
    clock_gettime(CLOCK_MONOTONIC, &next);
    last = next;
    do {
        struct timespec pop_begin;

        if (change) {
            clock_gettime(CLOCK_MONOTONIC, &pop_begin);
            test = eventReader1.pop(event,empty, gap);
        } else {
            clock_gettime(CLOCK_MONOTONIC, &pop_begin);
            test = eventReader2.pop(event, empty, gap);
        }
        struct timespec pop_end;
        clock_gettime(CLOCK_MONOTONIC, &pop_end);

        if(test!=0){
            std::cerr << "raven pop failed at "<<  __FILE__ << " line:" << __LINE__ << " error: " << strerror(errno) << std::endl;
            stats->increment_number_of_errors();
        }

        reader_read_checks(stats, counter, event, last_read_timestamp, empty, gap, pop_begin, pop_end);

        if (!empty) change = !change;

        reader_schedule(stats, next, last);
    } while (counter < end);
}

void SynchronizedEventReaderMultiThreadTest::writer_function(MultiThreadTestStats & stats) {
    StressData<STRESS_DATA_SIZE> data;
    EventWriter<StressData<STRESS_DATA_SIZE> > writer[2];
    writer[0].configure(buffer1.id);
    writer[1].configure(buffer2.id);
    unsigned char index = 0;
    struct timespec next, last;
    clock_gettime(CLOCK_MONOTONIC, &next);
    last = next;

    for (unsigned long long i = 0; i < NUMBER_OF_EVENTS; i++) {
        struct timespec push_begin;
        clock_gettime(CLOCK_MONOTONIC, &push_begin);
        int op=writer[index].push(data);
        struct timespec push_end;
        clock_gettime(CLOCK_MONOTONIC, &push_end);
        if (op != 0) {
            std::cerr << "raven_buffer_push failed at "<<  __FILE__ << " line:" << __LINE__ << " error: " << strerror(errno) << std::endl;
            stats.increment_number_of_errors();
        }
        stats.update_op_time(push_begin, push_end);
        writer_schedule(&stats, next, last);
        data.increment();
        if (index != 0)
            index = 0;
        else
            index = 1;
    }
}


std::string  SynchronizedEventReaderMultiThreadTest::get_test_name() const{
    return "Synchronized Event Reader Multithread test";
}

