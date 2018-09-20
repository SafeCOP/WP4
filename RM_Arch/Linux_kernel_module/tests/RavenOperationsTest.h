#ifndef KERNEL_RAVENOPERATIONSTEST_H
#define KERNEL_RAVENOPERATIONSTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
 * Tests the raven module operations.
 */
class RavenOperationsTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(RavenOperationsTest);
        CPPUNIT_TEST(testGetInfo);
        CPPUNIT_TEST(testList);
        CPPUNIT_TEST(testCreateAndDestroy);
        CPPUNIT_TEST(testListZeroLengthArray);
        CPPUNIT_TEST(testListOverflow);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();

    void tearDown();

    /**
     * Tests buffer create and destroy.
     */
    void testCreateAndDestroy();

    /**
     * Tests buffer get into.
     */
    void testGetInfo();

    /**
     *  Tests buffer list.
     */
    void testList();

    /**
     *  Tests buffer list with a zero length array.
     */
    void testListZeroLengthArray();

    /**
     * Tests that buffer list does not overflow if the array is too small to contain all the buffers.
     */
    void testListOverflow();
};


#endif //KERNEL_RAVENOPERATIONSTEST_H
