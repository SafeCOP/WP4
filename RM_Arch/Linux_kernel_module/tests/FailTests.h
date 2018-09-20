#ifndef KERNEL_FAILTESTS_H
#define KERNEL_FAILTESTS_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
 * Tests what happens when wrong inputs are given to the raven module.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
class FailTests : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(FailTests);
        CPPUNIT_TEST(testOverflowBufferDataBlockSize);
        CPPUNIT_TEST(testDuplicateBufferCreation);
        CPPUNIT_TEST(testZeroLengthBufferCreation);
        CPPUNIT_TEST(testCallOpsOnNonExistantBuffers);
        CPPUNIT_TEST(testOverflowIndex);
        CPPUNIT_TEST(testLastReadLargerThanSynch);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();

    /**
     * Test that an error code is returned when we try
     * to allocate a buffer with an index size larger than what is allowed.
     */
    void testOverflowBufferDataBlockSize();

    /**
     * Tests that an error code is returned when we try to allocate a buffer with an existing ID.
     */
    void testDuplicateBufferCreation();

    /**
     * Tests that an error is returned when we try to create a buffer whose index_length or length is zero.
     */
    void testZeroLengthBufferCreation();

    /**
     * Test that an error code is returned when we try to call operations on buffers that do now exist.
     */
    void testCallOpsOnNonExistantBuffers();

    /**
     * Tests that an error code is returned when we try change the last read index on an event reader to an invalid one.
     */
    void testOverflowIndex();

    /**
     * Tests what happens when the last read variable is larger than the sync variable.
     */
    void testLastReadLargerThanSynch();

    void tearDown();
};


#endif //KERNEL_FAILTESTS_H
