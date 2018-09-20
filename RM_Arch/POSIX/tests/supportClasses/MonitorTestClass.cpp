#include "MonitorTestClass.h"

MonitorTestClass::MonitorTestClass(IEventBuffer<int> &buffer)
        : Monitor(1, 0, SCHED_FIFO, 2), success(0) {
    configSynchronizedEventReader<int>(eventReader, buffer);
}

void MonitorTestClass::run() {
    Event<int> data;
    bool gap;
    if (eventReader.popHead(data, gap))
        success = true;
}


bool MonitorTestClass::getSuccess() {
    return success;
}
