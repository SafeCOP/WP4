#ifndef _MONITOR_BUFFERTEST_H_
#define _MONITOR_BUFFERTEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
 * Tests buffer operations.
 */
class BufferTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(BufferTest);
        CPPUNIT_TEST(testIsEmpty);
        CPPUNIT_TEST(testGetLength);
        CPPUNIT_TEST(testBuffer);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();

    void tearDown();

    /**
     * Tests getLength method.
     */
    void testGetLength();

    /**
     * tests buffer isEmpty method.
     */
    void testIsEmpty();

    /**
     * Tests buffer push/pop operations.
     */
    void testBuffer();
};


#endif //_MONITOR_BUFFERTEST_H_
