/**
 * @file api_types.h
 *
 * This file declares the api struct types to communicate with the linux kernel.
 *
 * Note this file should be kept updated on both kmod/api_types and lib/c/api_types.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */

#ifndef KERNEL_API_TYPES_H
#define KERNEL_API_TYPES_H

#ifndef __KERNEL__
#include <time.h>
#include <sys/types.h>
#endif /* __KERNEL__*/

/*
 * ===============================================
 *             Public API Definitions
 * ===============================================
 */

/**
 * Represents an event buffer
 */
struct event_buffer {
    /** the id of the buffer */
    key_t id;
    /** the number of indexes of the buffer */
    unsigned int length;
    /** how many bytes each index takes */
    unsigned short index_size;
};

/**
 * Information about a specific event buffer
 */
struct event_buffer_info {
    /** the last written timestamp */
    struct timespec last_written;
    /** the last overwritten timestamp*/
    struct timespec last_overwritten;
    /** the buffer id */
    key_t id;
    /** The pid of the process that owns this buffer */
    pid_t owner;
    /** the number of indexes of the buffer */
    unsigned int length;
    /** how many bytes each index takes */
    unsigned short index_size;
};

/**
 * Information about a list of event buffers
 */
struct event_buffer_list {
    /** A pointer to an array of event_buffer_info that contains information about each event buffer */
    struct event_buffer_info *event_buffers;
    /** the event_buffers array length  */
    size_t event_buffers_length;
    /** how many event_buffers exist */
    size_t number_of_buffers;
};

/**
 * Represents an event reader.
 *
 * There are two types of readers:
 *
 * - A synchronized reader uses a synchronization variable shared between buffers, events popped from these synchronized event readers are always later in time than the last one popped in any reader. The synch variable points to the address of the shared time stamp.
 * - A regular reader is isolated from other buffers, events popped from this reader are always larger than the last one popped from this buffer. The synch variable is NULL.
 *
 * The event_data length is not saved since the kernel land knows how big each index is.
 *
 * @warning Users should be careful modifying these variables as they can result in undefined behavior.
 */
struct event_reader {
    /** the time of the read event */
    struct timespec event_time;
    /** the time of the last event marked as read*/
    struct timespec last_read;
    /** a pointer to the event data */
    void *event_data;
    /** a pointer to the synchronization variable. NULL if this buffer isn't a synchronized reader */
    struct timespec *synch;
    /** the id of the buffer to pop events from */
    key_t id;
    /** the index this event buffer points at */
    unsigned int index;
    /** if there is a gap in the trace. that is if elements have been overwritten without us reading them */
    unsigned char gap;
    /** if the buffer is empty */
    unsigned char empty;
};

/**
 * Represents an event writer.
 *
 *  The data length is not saved since kernel land knows how big each index is.
 */
struct event_writer {
    /** A pointer to the data being saved */
    void *data;
    /** The id of the buffer to write events to */
    key_t buffer_id;
};

/*
 * ===============================================
 *             IOCTL Functions Declaration
 * ===============================================
 */
/** The Magic Number is a unique number or character that will differentiate our set of ioctl calls from the other ioctl calls. */
#define RAVEN_IOC_MAGIC ']'
/** IOCTL Command code to push an event to a buffer */
#define RAVEN_IOC_PUSH      _IOW(RAVEN_IOC_MAGIC, 0, struct event_writer *)
/** IOCTL Command code to pop an event from a buffer */
#define RAVEN_IOC_POP       _IOWR(RAVEN_IOC_MAGIC, 1, struct event_reader *)
/** IOCTL Command code to pop the newest event from a buffer */
#define RAVEN_IOC_POP_HEAD  _IOWR(RAVEN_IOC_MAGIC, 2, struct event_reader *)
/** IOCTL Command code to peek an event from a buffer */
#define RAVEN_IOC_PEEK       _IOWR(RAVEN_IOC_MAGIC, 3, struct event_reader *)
/** IOCTL Command code to peek the newest event from a buffer */
#define RAVEN_IOC_PEEK_HEAD  _IOWR(RAVEN_IOC_MAGIC, 4, struct event_reader *)
/** IOCTL Command code to create a buffer */
#define RAVEN_IOC_CREATE    _IOWR(RAVEN_IOC_MAGIC, 5, struct event_buffer *)
/** IOCTL Command code to destroyBuffer a buffer */
#define RAVEN_IOC_DESTROY   _IOW(RAVEN_IOC_MAGIC, 6, key_t)
/** IOCTL Command code to change the buffers owner to the calling process */
#define RAVEN_IOC_ACQUIRE _IOW(RAVEN_IOC_MAGIC, 7, key_t)
/** IOCTL Command code to get information about a buffer */
#define RAVEN_IOC_INFO      _IOWR(RAVEN_IOC_MAGIC, 8, struct event_buffer_info *)
/** IOCTL Command code to get information about all buffers */
#define RAVEN_IOC_LIST      _IOR(RAVEN_IOC_MAGIC, 9, struct event_buffer_list *)
/** The ID of the last IOCTL command code */
#define RAVEN_IOC_MAXNR 9

#endif //KERNEL_API_TYPES_H
