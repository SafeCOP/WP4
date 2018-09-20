#ifndef MONITOR_MONITORTEST_H
#define MONITOR_MONITORTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

/**
 * Tests that the monitor is working correctly.
 */
class MonitorTest : public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(MonitorTest);
        CPPUNIT_TEST(testMonitor);
    CPPUNIT_TEST_SUITE_END();

    void testMonitor();
};


#endif //MONITOR_MONITORTEST_H
