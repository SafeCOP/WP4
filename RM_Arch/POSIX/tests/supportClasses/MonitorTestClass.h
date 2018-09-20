#ifndef MONITOR_MONITORTESTCLASS_H
#define MONITOR_MONITORTESTCLASS_H


#include "../../Monitor.h"

class MonitorTestClass : public Monitor {
private:
    bool success;
    SynchronizedEventReader<int> eventReader;
protected:
    void run();

public:
    MonitorTestClass(IEventBuffer<int> &buffer);

    bool getSuccess();
};


#endif //MONITOR_MONITORTESTCLASS_H
