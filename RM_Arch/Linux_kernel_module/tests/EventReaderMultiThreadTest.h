#ifndef KERNEL_EVENTREADERMULTITHREADTEST_H
#define KERNEL_EVENTREADERMULTITHREADTEST_H

#include "supportClasses/MultiThreadTestTemplate.h"
#include "../lib/c/raven_api.h"
#include "supportClasses/StressData.h"

/**
 * Event Reader Multi thread test.
 *
 * In this test a writer pushes elements to a buffer and a reader pops elements from a buffer ensuring they are sequentially correct.
 *
 * @see MultiThreadTestTemplate
 */
class EventReaderMultiThreadTest : public MultiThreadTestTemplate, public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(EventReaderMultiThreadTest);
        CPPUNIT_TEST(run);
    CPPUNIT_TEST_SUITE_END();
private:
    struct event_buffer buffer;

    void init();

    void term();

    void writer_function(MultiThreadTestStats & stats);

    void reader_function(MultiThreadTestReaderStats * stats);

    std::string get_test_name() const;

public:
    EventReaderMultiThreadTest();
};


#endif //KERNEL_EVENTREADERMULTITHREADTEST_H
