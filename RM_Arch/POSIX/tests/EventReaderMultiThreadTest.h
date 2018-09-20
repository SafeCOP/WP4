#ifndef MONITOR_EVENTREADERMULTITHREADTEST_H
#define MONITOR_EVENTREADERMULTITHREADTEST_H

#include "supportClasses/MultiThreadTestTemplate.h"
#include "supportClasses/StressData.h"
#include "../EventBuffer.h"
#include "supportClasses/MultiThreadTestReaderStats.h"

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
    EventBuffer<StressData<STRESS_DATA_SIZE>, BUFFER_SIZE> buf;

    MultiThreadTestStats *writer_function();

    MultiThreadTestReaderStats *reader_function();

    std::string get_test_name() const;
};


#endif //MONITOR_EVENTREADERMULTITHREADTEST_H
