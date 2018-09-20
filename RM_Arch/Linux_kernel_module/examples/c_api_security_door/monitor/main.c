#include <stdio.h> //printf, perror
#include <stdlib.h> //exit
#include <sys/ipc.h> //ftok
#include <unistd.h> //sleep
#include <signal.h> //signal

#include "../../../lib/c/raven_api.h"
#include "shared_data.h"

volatile sig_atomic_t exit_flag = 0;

void sig_int_handler(int sig) {
    exit_flag = 1; // set the exit flag
}

void timespec_diff(struct timespec *start, struct timespec *stop, struct timespec *result) {
    if (stop->tv_nsec - start->tv_nsec < 0) {
        result->tv_sec = stop->tv_sec - start->tv_sec - 1;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        result->tv_sec = stop->tv_sec - start->tv_sec;
        result->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }
}

key_t connect_event_buffer(char key) {
    // Open a file descriptor to /dev/raven
    int test = raven_init();
    if (test < 0) {
        perror("raven_init failed");
        exit(1);
    }
    // get the buffer ID generated from ftok
    key_t id = ftok("/dev/null", key);
    if (id == -1) {
        perror("ftok failed");
        exit(1);
    }

    return id;
}

void close_raven_api() {
    // close the file descriptor to /dev/raven
    raven_term();
}


void close_door(int *door) {
    __atomic_store_n(door, DOOR_CLOSED, __ATOMIC_SEQ_CST);
    printf("Door closed!\n");
}

void monitor(key_t door_opened_id, key_t door_closed_id, int *door) {
    struct timespec synch = {0, 0};

    // Initialize the event reader
    struct event_reader door_open_reader;
    int door_open_status = 0;
    raven_init_event_reader(&door_open_reader, door_opened_id, &door_open_status, &synch);

    // Initialize the event reader
    struct event_reader door_closed_reader;
    int door_closed_status = 0;
    raven_init_event_reader(&door_closed_reader, door_closed_id, &door_closed_status, &synch);

    int door_status = DOOR_CLOSED;
    //todo gaps
    printf("Monitor is active\n");
    for (; ;) {
        // Pop an event from the event buffer
        int test = raven_pop(&door_open_reader); //can call raven_pop_head, raven_peek, and raven_peek_head see the doc for more info
        if (test < 0) {
            perror("raven_buffer_pop failed");
            break;
        }

        // check if the buffer was empty
        if (!door_open_reader.empty) {
            if(door_open_reader.gap){
                printf("Door closed reader: a gap occurred\n");
            }
            door_status = DOOR_OPENED;
        }

        if(door_status == DOOR_OPENED) {
            // Pop an event from the event buffer
            test = raven_pop(&door_closed_reader);
            if (test < 0) {
                perror("raven_buffer_pop failed");
                break;
            }

            // check if the buffer was empty
            if (door_closed_reader.empty) {
                struct timespec current_time;
                test = clock_gettime(CLOCK_MONOTONIC, &current_time);
                if (test == -1) {
                    perror("clock_gettime failed");
                    break;
                }

                //has the door been open for longer than a second?
                struct timespec diff;
                timespec_diff(&door_open_reader.last_read, &current_time, &diff);
                if (diff.tv_sec >= 1) {
                    printf("Door has been open for more than 1 second, closing\n");
                    close_door(door);
                    door_status = DOOR_CLOSED;
                }

                if(door_closed_reader.gap){
                    printf("Door closed reader: a gap occurred\n");
                }
            } else {
                door_status=DOOR_CLOSED;

                //how long was the door open?
                struct timespec diff;
                timespec_diff(&door_open_reader.event_time, &door_closed_reader.event_time, &diff);
                if (diff.tv_sec >= 1) {
                    printf("Door was open for more than 1 second !\n");
                }
            }

        }

        if (exit_flag) break;
        sleep(1);
    }
}


int main(int argc, char *argv[]) {
    //set the signal interrupt handler
    signal(SIGINT, sig_int_handler);

    int *door = config_shared_memory();
    key_t door_opened_id = connect_event_buffer('s');
    key_t door_closed_id = connect_event_buffer('d');

    monitor(door_opened_id, door_closed_id, door);

    close_raven_api();
    free_shared_memory();

    return 0;
}
