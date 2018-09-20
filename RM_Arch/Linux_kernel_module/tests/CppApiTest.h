#ifndef KERNEL_CPPAPITEST_H
#define KERNEL_CPPAPITEST_H


#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
 * Tests the C++ wrapper API.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
class CppApiTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(CppApiTest);
        CPPUNIT_TEST(buildEventBufferTest);
        CPPUNIT_TEST(eventBufferBuilderInitializeFactoryTest);
        CPPUNIT_TEST(eventReaderTest);
    CPPUNIT_TEST(synchronizedEventReaderTest);
    CPPUNIT_TEST(acquireTest);
    CPPUNIT_TEST_SUITE_END();

public:
    /**
     * Tests the BuildEventBuffer functions.
     */
    void buildEventBufferTest();

    /**
     * Tests the EventBuffer initialize functions.
     */
    void eventBufferBuilderInitializeFactoryTest();

    /**
     * Tests the EventReader c++ API.
     */
    void eventReaderTest();

    /**
     * Tests the SynchronizedEventReader C++ API.
     */
    void synchronizedEventReaderTest();

    /**
     * Tests the C++ API acquire op.
     */
    void acquireTest();
};

#endif //KERNEL_CPPAPITEST_H
