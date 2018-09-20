#include "MonitorTest.h"
#include "supportClasses/MonitorTestClass.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MonitorTest);


void MonitorTest::testMonitor() {
    EventBuffer<int, 3> eventBuffer;
    EventWriter<int> eventWriter;
    eventBuffer.configWriter(eventWriter);
    eventWriter.push(1);

    MonitorTestClass monitor(eventBuffer);
    CPPUNIT_ASSERT(monitor.enableWithDeadlineMissCheck() == 0);
    monitor.synch_disable();
    CPPUNIT_ASSERT(monitor.getSuccess());
}
