/**
 * @file raven_api.c
 *
 * This file contains all the raven API function declarations such as raven_init, raven_pop, and raven_push.
 *
 * @see raven_api.h
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */

#include <sys/ioctl.h>
#include <unistd.h> /* close */
#include <fcntl.h>      /* open */

#include "raven_api.h"

/** The file descriptor ID to /dev/raven */
static int fd=-1;

int raven_init() {
    fd = open("/dev/raven", 'w');
    return fd;
}

int raven_term() {
    int ret = close(fd);
    if (ret == 0) fd = -1;
   return ret;
}

int raven_list_buffers(struct event_buffer_list *buffer_list, struct event_buffer_info *event_buffer_array,
                       size_t array_length) {
    buffer_list->event_buffers = event_buffer_array;
    buffer_list->event_buffers_length = array_length;
    return ioctl(fd, RAVEN_IOC_LIST, buffer_list);
}

int raven_buffer_info(key_t buffer_id, struct event_buffer_info *event_buffer) {
    event_buffer->id = buffer_id;
    return ioctl(fd, RAVEN_IOC_INFO, event_buffer);
}

int raven_buffer_create(key_t id, unsigned int length, unsigned short index_size) {
    struct event_buffer event_buffer;
    event_buffer.id = id;
    event_buffer.index_size = index_size;
    event_buffer.length = length;
    return ioctl(fd, RAVEN_IOC_CREATE, &event_buffer);
}

int raven_buffer_destroy(key_t id) {
    return ioctl(fd, RAVEN_IOC_DESTROY, id);
}


int raven_push(struct event_writer *buffer_push) {
    return ioctl(fd, RAVEN_IOC_PUSH, buffer_push);
}

int raven_pop(struct event_reader *buffer_pop) {
    return ioctl(fd, RAVEN_IOC_POP, buffer_pop);
}

int raven_pop_head(struct event_reader *buffer_pop) {
    return ioctl(fd, RAVEN_IOC_POP_HEAD, buffer_pop);
}

int raven_peek(struct event_reader *buffer_pop) {
    return ioctl(fd, RAVEN_IOC_PEEK, buffer_pop);
}

int raven_peek_head(struct event_reader *buffer_pop) {
    return ioctl(fd, RAVEN_IOC_PEEK_HEAD, buffer_pop);
}

int raven_acquire(key_t buffer_id) {
    return ioctl(fd, RAVEN_IOC_ACQUIRE, buffer_id);
}

void raven_init_event_reader(struct event_reader *pop, key_t id, void *data, struct timespec *synch) {
    pop->id = id;
    pop->event_data = data;
    pop->index = 1;
    pop->last_read.tv_nsec = 0;
    pop->last_read.tv_sec = 0;
    pop->synch = synch;

    //Initialization of these variables is not needed, but doing so removes valgrind memory errors
    pop->gap = 0;
    pop->empty = 1;
    pop->event_time.tv_nsec = 0;
    pop->event_time.tv_nsec = 0;
}

void raven_init_event_writer(struct event_writer *writer, key_t buffer_id, void *data) {
    writer->buffer_id = buffer_id;
    writer->data = data;
}
