#ifndef KERNEL_EVENTREADERTEST_H
#define KERNEL_EVENTREADERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
 * Tests the event reader.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
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
        CPPUNIT_TEST(testPopHead);
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

    void testPopHead();

    void tearDown();
};


#endif //KERNEL_EVENTREADERTEST_H
