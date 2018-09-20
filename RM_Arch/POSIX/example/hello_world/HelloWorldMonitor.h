//
// Created by silverone on 1/19/16.
//

#ifndef MONITOR_HELLOWORLDMONITOR_H
#define MONITOR_HELLOWORLDMONITOR_H

#include "../../Monitor.h"
#include "../../EventReader.h"

using namespace std;

class HelloWorldMonitor : public Monitor {
private:
    EventReader<string> eventReader;

public:
    HelloWorldMonitor(const IEventBuffer<string> & buffer) : Monitor(1,0){
        buffer.configReader(eventReader);
    }

    void run() {
        Event<string> event;
        bool gap;
        if(eventReader.pop(event, gap)){
            std::cout << "Popped event with data: [" << event.getData() << "] and time stamp: " <<
                    event.getTime().tv_sec << "s " << event.getTime().tv_nsec << " ns" << endl;

            if(gap){
                std::cout << "Oh noes! A gap occurred! We missed some elements!" << endl;
            }
        } else {
            std::cout << "Event buffer was empty" << endl;
        }
    }
};


#endif //MONITOR_HELLOWORLDMONITOR_H
