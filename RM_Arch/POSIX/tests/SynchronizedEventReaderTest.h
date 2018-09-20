#ifndef MONITOR_SYNCHRONIZEDEVENTREADERTEST_H
#define MONITOR_SYNCHRONIZEDEVENTREADERTEST_H


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
 * Tests the synchronized event reader
 */
class SynchronizedEventReaderTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(SynchronizedEventReaderTest);
        CPPUNIT_TEST(testNotFull1);
        CPPUNIT_TEST(testNotFull2);
        CPPUNIT_TEST(testNotFull3);
        CPPUNIT_TEST(testOverflow);
        CPPUNIT_TEST(testOverflow2);
        CPPUNIT_TEST(testOverflow3);
        CPPUNIT_TEST(testFull);
        CPPUNIT_TEST(testEmpty);
        CPPUNIT_TEST(testHead);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();

    void tearDown();

    void testNotFull1();

    void testNotFull2();

    void testNotFull3();

    void testOverflow();

    void testOverflow2();

    void testOverflow3();

    void testFull();

    void testEmpty();

    void testHead();
};


#endif //MONITOR_SYNCHRONIZEDEVENTREADERTEST_H

