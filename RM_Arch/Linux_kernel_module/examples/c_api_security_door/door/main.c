#include <stdio.h> //printf, perror
#include <stdlib.h> //exit
#include <sys/ipc.h> //ftok
#include <unistd.h> //sleep
#include <signal.h> //signal

#include "../../../lib/c/raven_api.h"
#include "shared_data.h"

#define BUFFER_SIZE 128

volatile sig_atomic_t exit_flag = 0;

void sig_int_handler(int sig) {
    exit_flag = 1; // set the exit flag
}

key_t create_event_buffer(char key) {
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

    // create the event buffer
    test = raven_buffer_create(id, BUFFER_SIZE, sizeof(int));
    if (test < 0) {
        test = raven_acquire(id);
        if (test < 0){
            perror("raven_buffer_create failed");
            exit(1);
        }
    }

    return id;
}

void deallocate_buffer(key_t id) {
    // destroy the event buffer
    int test = raven_buffer_destroy(id);
    if (test < 0) {
        perror("raven_destroy failed");
        exit(1);
    }

    // close the file descriptor to /dev/raven
    raven_term();
}

void producer(key_t id, int data) {
    // declare and initialize the event writer
    struct event_writer writer;
    raven_init_event_writer(&writer, id, &data);

    int test;
    // push data to the buffer
    test = raven_push(&writer);
    if (test < 0) {
        perror("raven_push failed");
    }
}

void open_door(key_t buffer_id, int *door) {
    __atomic_store_n(door, DOOR_OPENED, __ATOMIC_SEQ_CST);
    producer(buffer_id, DOOR_OPENED);
    printf("Door opened!\n");
}

void open_close_door(key_t door_opened_id, key_t door_closed_id, int *door) {
    open_door(door_opened_id, door);
    __atomic_store_n(door, DOOR_CLOSED, __ATOMIC_SEQ_CST);
    producer(door_closed_id, DOOR_CLOSED);

    sleep(1);
    printf("Door closed!\n");
}

int main(int argc, char *argv[]) {
    //set the signal interrupt handler
    signal(SIGINT, sig_int_handler);

    int *door = config_shared_memory();

    key_t door_opened_id = create_event_buffer('s');
    key_t door_closed_id = create_event_buffer('d');

    char input;
    for (; ;) {

        printf("[1] open/close door\n[2] open the door\n[3] exit\n[1,2,3]: ");
        scanf(" %c", &input);

        int door_status = __atomic_load_n(door, __ATOMIC_SEQ_CST);
        if (door_status == DOOR_OPENED) {
            printf("Door is currently open\n");
        } else {
            printf("Door is currently closed\n");
        }

        if (input == '1') {
            open_close_door(door_opened_id, door_closed_id, door);
        } else if (input == '2') {
            open_door(door_opened_id, door);
        } else if (input == '3') {
            break;
        }

        if (exit_flag) break;
    }

    deallocate_buffer(door_opened_id);
    deallocate_buffer(door_closed_id);
    free_shared_memory();

    return 0;
}
