//
// Created by silverone on 8/18/15.
//

#ifndef MONITOR_ARRAYSEQLOCKTEST_H
#define MONITOR_ARRAYSEQLOCKTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
 * Tests the ArraySeqLock by testing all possible write/read lock combinations.
 */
class ArraySeqLockTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(ArraySeqLockTest);
        CPPUNIT_TEST(writer_before_before);
        CPPUNIT_TEST(writer_before_equal);
        CPPUNIT_TEST(writer_before_after);
        CPPUNIT_TEST(writer_equal_after);
        CPPUNIT_TEST(writer_after_after);
        CPPUNIT_TEST(writer_wrap_before_before);
        CPPUNIT_TEST(writer_wrap_before_equal);
        CPPUNIT_TEST(writer_wrap_before_after);
        CPPUNIT_TEST(writer_wraps_equal_before);
        CPPUNIT_TEST(writer_wraps_equal_equal);
        CPPUNIT_TEST(writer_wrap_equal_after);
        CPPUNIT_TEST(writer_wrap_after_after);
        CPPUNIT_TEST(writer_wrap_after_before);
        CPPUNIT_TEST(writer_wrap_after_equal);
        CPPUNIT_TEST(writer_wraps_many);
    CPPUNIT_TEST_SUITE_END();

public:
    void writer_before_before();

    void writer_before_equal();

    void writer_before_after();

    void writer_equal_after();

    void writer_after_after();

    void writer_wrap_before_before();

    void writer_wrap_before_equal();

    void writer_wrap_before_after();

    void writer_wraps_equal_before();

    void writer_wraps_equal_equal();

    void writer_wrap_equal_after();

    void writer_wrap_after_after();

    void writer_wrap_after_before();

    void writer_wrap_after_equal();

    void writer_wraps_many();
};


#endif //MONITOR_ARRAYSEQLOCKTEST_H
