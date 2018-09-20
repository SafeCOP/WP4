#ifndef MONITOR_EVENTREADERTEST_H
#define MONITOR_EVENTREADERTEST_H


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
 * Tests the EventReaders.
 */
class EventReaderTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(EventReaderTest);
        CPPUNIT_TEST(testEmpty);
        CPPUNIT_TEST(testHalfFull);
        CPPUNIT_TEST(testFullOne);
        CPPUNIT_TEST(testFullTwo);
        CPPUNIT_TEST(testFullThree);
        CPPUNIT_TEST(testFullFour);
        CPPUNIT_TEST(testFullFive);
        CPPUNIT_TEST(testPopPeekHead);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();

    void testEmpty();

    void testHalfFull();

    void testFullOne();

    void testFullTwo();

    void testFullThree();

    void testFullFour();

    void testFullFive();

    void testPopPeekHead();

    void tearDown();
};

#endif //MONITOR_EVENTREADERTEST_H
