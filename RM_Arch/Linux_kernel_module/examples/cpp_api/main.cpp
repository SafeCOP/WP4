#include <stdio.h> //printf, perror
#include <stdlib.h> //exit
#include <signal.h> //signal
#include <sys/ipc.h> //ftok
#include <errno.h> //errno
#include <string.h> //strerrno
#include <unistd.h> //sleep
#include <pthread.h> // POSIX Threads

#include "../../lib/cpp/raven_cpp_api.h"

using namespace std;

#define BUFFER_SIZE 128

volatile sig_atomic_t exit_flag = 0;
void sig_int_handler(int sig){
    exit_flag = 1; // set the exit flag
}

EventBuffer<char[250]> allocate_event_buffer(){
    EventBufferFactory<char[250]> factory;

    // Open a file descriptor to /dev/raven
    if(factory.initialize()==-1){ //must only be called once for all factories!
        perror("initialize failed");
        exit(1);
    }

    //declare the event buffer
    EventBuffer<char[250]> event_buffer;       //ftok params
    if(factory.buildEventBuffer(event_buffer, "/dev/null" ,'c', BUFFER_SIZE)==-1){
        perror("build event buffer failed");
        exit(1);
    }

    return event_buffer;
}

void deallocate_buffer(EventBuffer<char[250]> & buffer){
    // destroy the event buffer
    buffer.destroy();

    // close the file descriptor to /dev/raven, must only be called once for all factories!
    Factory::terminate();
}

void producer(EventBuffer<char[250]> & event_buffer){

    // declare and initialize the event writer
    EventWriter<char[250]> event_writer;
    event_buffer.configureWriter(event_writer);

    char data [250] = "Hello World!";

    while(!exit_flag){
        sleep(1);
        // push data to the buffer
        //push an element to the buffer
        if(event_writer.push_ref(data)!=0){
            perror("push failed");
            exit_flag=1;
            break;
        }
    }
}

void * consumer(void * event_buffer_ptr){
    EventBuffer<char[250]> * event_buffer = (EventBuffer<char[250]> *) event_buffer_ptr;

    // declare and initialize the event reader
    EventReader<char[250]> event_reader;
    event_buffer->configureReader(event_reader);

    int test;
    Event<char[250]> event;
    bool empty;
    bool gap;

    while(!exit_flag){
        sleep(1);

        // Pop an event from the event buffer
        test=event_reader.pop(event, empty, gap);
        if(test==-1){
            perror("event reader pop failed");
            exit_flag=1;
            break;
        }

        if (!empty) {
            std::cout << "Popped event with data: [" << event.getData() << "] and time stamp: " <<
            event.getTime().tv_sec << "s " << event.getTime().tv_nsec << " ns" << endl;

            if (gap) {
                std::cout << "Oh noes! A gap occurred! We missed some elements!" << endl;
            }
        } else {
            std::cout << "Event buffer was empty" << endl;
        }

    }
}

int main(int argc, char *argv[]) {
    //set the signal interrupt handler, event buffers must be deallocated, otherwise they will persist in kernel memory.
    signal(SIGINT, sig_int_handler);

    EventBuffer<char[250]> event_buffer = allocate_event_buffer();
    //create the consumer thread, the monitor
    pthread_t consumer_thread;
    pthread_create (&consumer_thread, NULL, consumer, (void *) &event_buffer);

    //push events to the buffer
    producer(event_buffer);

    //join with the consumer
    pthread_join(consumer_thread, NULL);

    deallocate_buffer(event_buffer);

    return 0;
}
