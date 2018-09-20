#ifndef KERNEL_SYNCHRONIZEDEVENTREADERMULTITHREADTEST_H
#define KERNEL_SYNCHRONIZEDEVENTREADERMULTITHREADTEST_H

#include "supportClasses/MultiThreadTestTemplate.h"
#include "../lib/c/raven_api.h"
#include "supportClasses/StressData.h"

/**
 * Synchronized Event Reader Multi thread test.
 *
 * In this test a writer alternates between two buffers, readers pops elements from them in an alternating fashion, while ensuring they are sequentially correct.
 *
 * @see MultiThreadTestTemplate
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
class SynchronizedEventReaderMultiThreadTest : public MultiThreadTestTemplate, public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SynchronizedEventReaderMultiThreadTest);
        CPPUNIT_TEST(run);
    CPPUNIT_TEST_SUITE_END();
private:
    struct event_buffer buffer1;
    struct event_buffer buffer2;

    void init();

    void term();

    void writer_function(MultiThreadTestStats & stats);

    void reader_function(MultiThreadTestReaderStats * stats);

    std::string get_test_name() const;

public:
    SynchronizedEventReaderMultiThreadTest();
};


#endif //KERNEL_SYNCHRONIZEDEVENTREADERMULTITHREADTEST_H
