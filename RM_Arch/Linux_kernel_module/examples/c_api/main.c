#include <stdio.h> //printf, perror
#include <stdlib.h> //exit
#include <signal.h> //signal
#include <sys/ipc.h> //ftok
#include <errno.h> //errno
#include <string.h> //strerrno
#include <unistd.h> //sleep
#include <pthread.h> // POSIX Threads

#include "../../lib/c/raven_api.h"

#define BUFFER_SIZE 128

volatile sig_atomic_t exit_flag = 0;
void sig_int_handler(int sig){
    exit_flag = 1; // set the exit flag
}

key_t create_event_buffer(){
    // Open a file descriptor to /dev/raven
    int test=raven_init();
    if(test<0){
        perror("raven_init failed");
        exit(1);
    }
    // get the buffer ID generated from ftok
    key_t id = ftok("/dev/null", 's');
    if(id==-1){
        perror("ftok failed");
        exit(1);
    }

    // create the event buffer
    test=raven_buffer_create(id, BUFFER_SIZE, sizeof(char[254]));
    if(test<0){
        perror("raven_buffer_create failed");
        exit(1);
    }

    return id;
}

void deallocate_buffer(key_t id){
    // destroy the event buffer
    int test=raven_buffer_destroy(id);
    if(test<0){
        perror("raven_destroy failed");
        exit(1);
    }

    // close the file descriptor to /dev/raven
    raven_term();
}

void producer(key_t id){
    // declare and initialize the event writer
    struct event_writer writer;
    char data [250] = "Hello World!";
    raven_init_event_writer(&writer, id, &data);

    int test;
    while(!exit_flag){
        sleep(1);
        // push data to the buffer
        test = raven_push(&writer);
        if (test < 0) {
            perror("raven_push failed");
            exit_flag=1;
            break;
        }
    }
}

void * consumer(void * buffer_id){
    key_t id = (key_t) (long) buffer_id;
    int test;

    // declare and initialize the event reader
    struct event_reader reader;
    char read_data[250];
    raven_init_event_reader(&reader, id, &read_data, NULL);

    while(!exit_flag){
        sleep(1);

        // Pop an event from the event buffer
        test=raven_pop(&reader); //can call raven_pop_head, raven_peek, and raven_peek_head see the doc for more info
        if(test<0){
            perror("raven_buffer_pop failed");
            exit_flag=1;
            break;
        }
        // check if the buffer was empty
        if(!reader.empty){
            printf("read: [%s] with timestamp: %lld.%.9ld, last read timestamp: %lld.%.9ld\n", read_data, (long long)reader.event_time.tv_sec, reader.event_time.tv_nsec,  (long long)reader.last_read.tv_sec, reader.last_read.tv_nsec);
            // check if an overflow occurred
            if(reader.gap)
                printf("Oh no! A gap occurred ! we lost some events!\n");
        } else {
            printf("Buffer was empty!\n");
        }
    }
    
    return NULL;
}

int main(int argc, char *argv[]) {
    //set the signal interrupt handler, event buffers must be deallocated, otherwise they will persist in kernel memory.
    signal(SIGINT, sig_int_handler);

    key_t id = create_event_buffer();

    //create the reader thread, the monitor
    pthread_t consumer_thread;
    pthread_create (&consumer_thread, NULL, consumer, (void *) (long) id);

    //push events to the event buffer
    producer(id);

    pthread_join(consumer_thread, NULL);

    deallocate_buffer(id);

    return 0;
}
