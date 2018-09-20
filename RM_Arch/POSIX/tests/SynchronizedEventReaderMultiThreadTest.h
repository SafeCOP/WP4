#ifndef MONITOR_SYNCHRONIZEDMULTITHREADTEST_H
#define MONITOR_SYNCHRONIZEDMULTITHREADTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "supportClasses/MultiThreadTestTemplate.h"
#include "../EventBuffer.h"
#include "supportClasses/StressData.h"
/**
 * Synchronized Event Reader Multi thread test.
 *
 * In this test a writer alternates between two buffers, readers pops elements from them in an alternating fashion, while ensuring they are sequentially correct.
 *
 * @see MultiThreadTestTemplate
 */
class SynchronizedEventReaderMultiThreadTest : public MultiThreadTestTemplate, public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SynchronizedEventReaderMultiThreadTest);
        CPPUNIT_TEST(run);
    CPPUNIT_TEST_SUITE_END();
private:
    EventBuffer<StressData<STRESS_DATA_SIZE>, BUFFER_SIZE> buf1;
    EventBuffer<StressData<STRESS_DATA_SIZE>, BUFFER_SIZE> buf2;

    MultiThreadTestStats *writer_function();

    MultiThreadTestReaderStats *reader_function();

    std::string get_test_name() const;
};


#endif //MONITOR_SYNCHRONIZEDMULTITHREADTEST_H
