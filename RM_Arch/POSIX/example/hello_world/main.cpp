//
// Created by silverone on 1/19/16.
//

#include "../../EventBuffer.h"
#include "HelloWorldMonitor.h"
#include <unistd.h>

using namespace std;

void producer(IEventBuffer<string> & buffer){
    EventWriter<string> event_writer;
    buffer.configWriter(event_writer);

    for(;;){
        event_writer.push("Hello World!");
        sleep(1);
    }
}

int main(int argc, char *argv[]) {
    EventBuffer<string, 30> eventBuffer;

    HelloWorldMonitor monitor(eventBuffer);
    monitor.enable();

    producer(eventBuffer);

    return 0;
}
