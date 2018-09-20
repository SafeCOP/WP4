#ifndef _MONITOR_EVENTTEST_H_
#define _MONITOR_EVENTTEST_H_

#include <cppunit/extensions/HelperMacros.h>

/**
 * Tests the events < > <= >= == != operators.
 */
class EventTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(EventTest);
        CPPUNIT_TEST(testBefore);
        CPPUNIT_TEST(testAfter);
        CPPUNIT_TEST(testEqual);
        CPPUNIT_TEST(testDiferent);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();

    void tearDown();

    void testBefore();

    void testAfter();

    void testEqual();

    void testDiferent();
};


#endif //_MONITOR_EVENTTEST_H_
