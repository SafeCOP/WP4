#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "MultiThreadTestTemplate.h"


MultiThreadTestTemplate::MultiThreadTestTemplate() : NUMBER_OF_EVENTS(NUMBER_OF_EVENTS_D),
                                                     NUMBER_OF_READERS(NUMBER_OF_READERS_D),
                                                     schedule_policy(SCHEDULE_POLICY), priority(PRIORITY), writer_stats(), active_readers(0){
    writer_period.tv_nsec = WRITER_PERIOD_NSEC;
    writer_period.tv_sec = WRITER_PERIOD_SEC;
    reader_period.tv_nsec = READER_PERIOD_NSEC;
    reader_period.tv_sec = READER_PERIOD_SEC;
}

MultiThreadTestTemplate::~MultiThreadTestTemplate() {
    munmap(shared_stats,sizeof(MultiThreadTestReaderStats)* NUMBER_OF_READERS);
}


void MultiThreadTestTemplate::run() {
    print_test_config();
    init();

    shared_stats = (MultiThreadTestReaderStats *) mmap(NULL, sizeof(MultiThreadTestReaderStats)* NUMBER_OF_READERS, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    CPPUNIT_ASSERT(shared_stats != MAP_FAILED);

    struct sched_param param1;
    param1.sched_priority = 99;


    if (sched_setscheduler(0, SCHEDULE_POLICY, &param1) != 0)
        std::cout << "Failed to raise process priority" << std::endl;

    for (size_t i = 0; i < NUMBER_OF_READERS; i++) {
        readers[i] = fork();
        if (readers[i] == 0) {
            // child process
            this->reader_function(new(&shared_stats[i]) MultiThreadTestReaderStats());
            shared_stats[i].~MultiThreadTestReaderStats();//this is necessary because of placement new
            exit(0);
        } else if (readers[i] < 0)
            std::cerr << "fork failed at " << __FILE__ << " " << __LINE__ << " error: " << strerror(errno) << std::endl;
        else
            ++active_readers;
    }

    struct timespec time1;
    clock_gettime(CLOCK_MONOTONIC, &time1);

    writer_function(writer_stats);

    for (size_t i = 0; i < active_readers; i++)
        wait(NULL);

    MultiThreadTestReaderStats reader_stats;

    for (size_t i = 0; i < active_readers; i++)
        reader_stats.join(shared_stats[i]);

    struct timespec time2;
    clock_gettime(CLOCK_MONOTONIC, &time2);

    term();

    print_test_stats(writer_stats, reader_stats, time2.tv_sec - time1.tv_sec);

    CPPUNIT_ASSERT(writer_stats.getNumber_of_errors() + reader_stats.getNumber_of_errors() == 0);
}


void MultiThreadTestTemplate::reader_schedule(MultiThreadTestReaderStats *stats, struct timespec &next,
                                              struct timespec &last) {
    if (reader_period.tv_nsec != 0 || reader_period.tv_sec != 0) {
        timeAdd(next, reader_period);
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        stats->update_loop_time(last, now);
        if (now > next)
            stats->increment_number_of_deadline_misses();
        else
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);

        clock_gettime(CLOCK_MONOTONIC, &last);
    } else {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        stats->update_loop_time(last, now);
        last = now;
    }
}

void MultiThreadTestTemplate::writer_schedule(MultiThreadTestStats *stats, struct timespec &next,
                                              struct timespec &last) {
    if (writer_period.tv_nsec != 0 || writer_period.tv_sec != 0) {
        timeAdd(next, writer_period);
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        stats->update_loop_time(last, now);
        if (now > next)
            stats->increment_number_of_deadline_misses();
        else
            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);

        clock_gettime(CLOCK_MONOTONIC, &last);
    } else {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        stats->update_loop_time(last, now);
        last = now;
    }
}

void MultiThreadTestTemplate::reader_read_checks(MultiThreadTestReaderStats *stats, unsigned long long &counter,
                                                 Event<StressData<STRESS_DATA_SIZE> > &event,
                                                 struct timespec &last_read_timestamp, bool &empty, bool &gap,
                                                 struct timespec &pop_begin, struct timespec &pop_end) {
    if (!empty) {
        if (gap) {
            stats->update_gap_time(pop_begin, pop_end);
            if (event.getTime() <= last_read_timestamp) {
                if (event.getData()[0] > counter) {
                    std::cout << "A gap occured, current timestamp is <= than the last read" << std::endl;
                } else {
                    std::cout <<
                    "A gap occured, current timestamp is <= than the last read and the data timestamp is <= than last read" <<
                    std::endl;
                }
                stats->increment_number_of_errors();
            }
            if (event.getData()[0] == counter) {
                stats->increment_number_of_false_gaps();
            } else {
                counter = event.getData()[0];
                stats->increment_number_of_gaps();
            }
        } else {
            stats->update_op_time(pop_begin, pop_end);
            if (event.getTime() <= last_read_timestamp) {
                std::cout << "Current timestamp is <= than the last read" << std::endl;
                stats->increment_number_of_errors();
            }
        }

        unsigned int errors = event.getData().check(counter);
        if (errors != 0)
            stats->increment_number_of_errors(errors);

        last_read_timestamp = event.getTime();
        stats->increment_number_of_ops();
    } else
        stats->increment_number_of_empty_reads();

}

void MultiThreadTestTemplate::print_test_config() {
    std::cout << std::endl << get_test_name() << std::endl << "Buffer Length: " << BUFFER_SIZE <<
    std::endl << "Event Number: " << NUMBER_OF_EVENTS << std::endl << "Number of readers: " << NUMBER_OF_READERS <<
    std::endl << "Scheduling policy: " << SCHEDULE_POLICY << std::endl << "Scheduling priority: " << priority <<
    std::endl << "Reader period: " << reader_period.tv_sec << "s " << reader_period.tv_nsec << "ns" << std::endl <<
    "Writer period: " << writer_period.tv_sec << "s " << writer_period.tv_nsec << "ns" << std::endl;
    std::cout << "Multi thread tests will begin now... this should take a while" << std::endl;
}


void MultiThreadTestTemplate::print_test_stats(const MultiThreadTestStats &writer_stats,
                                               const MultiThreadTestReaderStats &readerStats, const __time_t time) {
    std::cout << "Test Complete!" << std::endl;
    std::cout << "Number of gaps: " << readerStats.getNumber_of_gaps() << std::endl;
    std::cout << "Number of false gaps: " << readerStats.getNumber_of_false_gaps() << std::endl;
    std::cout << "Number of empty reads: " << readerStats.getNumber_of_empty_reads() << std::endl;
    if (WRITER_PERIOD_SEC != 0 || WRITER_PERIOD_NSEC != 0)
        std::cout << "Number of writer deadline misses: " << writer_stats.getNumber_of_deadline_misses() << std::endl;
    if (READER_PERIOD_SEC != 0 || READER_PERIOD_NSEC != 0)
        std::cout << "Number of reader deadline misses: " << readerStats.getNumber_of_deadline_misses() << std::endl;


    std::cout << "Number of errors: " << writer_stats.getNumber_of_errors() + readerStats.getNumber_of_errors() <<
    std::endl;


    std::cout << "min read time: " << readerStats.getMin_op_time().tv_sec << "s " <<
    readerStats.getMin_op_time().tv_nsec << "ns" << std::endl;

    std::cout << "max read time: " << readerStats.getMax_op_time().tv_sec << "s " <<
    readerStats.getMax_op_time().tv_nsec << "ns" << std::endl;

    if (readerStats.getNumber_of_gaps() != 0) {
        std::cout << "min gap read time: " << readerStats.getMin_gap_time().tv_sec << "s " <<
        readerStats.getMin_gap_time().tv_nsec << "ns" << std::endl;

        std::cout << "max gap read time: " << readerStats.getMax_gap_time().tv_sec << "s " <<
        readerStats.getMax_gap_time().tv_nsec << "ns" << std::endl;
    }

    std::cout << "min read loop time: " << readerStats.getMin_loop_time().tv_sec << "s " <<
    readerStats.getMin_loop_time().tv_nsec << "ns" << std::endl;

    std::cout << "max read loop time: " << readerStats.getMax_loop_time().tv_sec << "s " <<
    readerStats.getMax_loop_time().tv_nsec << "ns" << std::endl;

    std::cout << "min write time: " << writer_stats.getMin_op_time().tv_sec << "s " <<
    writer_stats.getMin_op_time().tv_nsec << "ns" << std::endl;

    std::cout << "max write time: " << writer_stats.getMax_op_time().tv_sec << "s " <<
    writer_stats.getMax_op_time().tv_nsec << "ns" << std::endl;

    std::cout << "min write loop time: " << writer_stats.getMin_loop_time().tv_sec << "s " <<
    writer_stats.getMin_loop_time().tv_nsec << "ns" << std::endl;

    std::cout << "max write loop time: " << writer_stats.getMax_loop_time().tv_sec << "s " <<
    writer_stats.getMax_loop_time().tv_nsec << "ns" << std::endl;

    std::cout << "Wrote " << NUMBER_OF_EVENTS << ", read " << readerStats.getNumber_of_ops() << " in " <<
    time << " seconds" << " or " << time / 60 << " minutes and " << time % 60 << " seconds." << std::endl;
}
