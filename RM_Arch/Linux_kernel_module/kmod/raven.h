/**
 * @file raven.h
 *
 * This file contains all the user land / kernel land structure declarations. EventReaders and Writers, IOCTL codes,
 * locking mechanisms, etc are defined here.
 *
 * A # ifdef __KERNEL__ is used to check if we compiling a kernel module or compiling an API on user land.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */

#ifndef KERNEL_RAVEN_H
#define KERNEL_RAVEN_H

#include "api_types.h"

#ifdef __KERNEL__

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/time.h>
#include <linux/seqlock.h>
#include <linux/ktime.h>
#include <linux/compat.h>
#include <linux/mutex.h>
#include <asm/page.h> /* PAGE_SIZE macro */

#include "array_seqlock.h"
/*
 * ===============================================
 *             Module Definitions
 * ===============================================
 */

/**
* The module name.
*/
#define MODULE_NAME "raven"

/**
 * The event_buffers_length of the hash table used to keep the event buffers. Will be 2^RAVEN_HASH_TABLE_SIZE or 2 to the power of the number
 * defined here.
 */
#define RAVEN_HASH_TABLE_SIZE 8

/**
 * The hash table used to keep the event buffers
 */
extern DECLARE_HASHTABLE(raven_hash_table, RAVEN_HASH_TABLE_SIZE);

/**
 * A mutual exclusion construct to prevent race conditions when creating and deleting event buffers. Only one of these
 * operations can run at any time.
 */
extern struct mutex raven_mutex;

/**
 * The number of buffers that currently exist.
 */
extern size_t number_of_buffers;


#if BITS_PER_LONG >= 64
    /**
     * The maximum index event_buffers_length for buffers in bytes. This is necessary because the data is temporarily stored on the
     * kernel thread stack. As this stack has a limit, we limit the stack event_buffers_length to 1928b on x64 and 956b on x32 machines.
     *
     * These values were calculated because the kernel developers have configured the gcc -Wstack-usage to emit warnings when
     * the stack event_buffers_length increases beyond the limit. Increasing these values even by a single byte results in a warning.
     */
    #define MAX_INDEX_SIZE 1928
#else
    #define MAX_INDEX_SIZE 956
#endif



/**
 * An event buffer on the kernel side.
 *
 * Represents an event buffer on the kernel side. This structure includes an hlist_node to allow for linked list insertion.
 * The structure itself also depends on the machines architecture, on x32 it contains an additional field: seqlock_t last_overwritten_lock. Because the last_overwritten ktime_t field is 8b long, writes/reads to the this variable are
 * not atomic on x32, needing a sequential lock.
 *
 * @warning Operation on event buffers such as any kind of writing and reading should be done exclusively by functions in
 * array_operations.h .
 */
struct raven_event_buffer {
#if BITS_PER_LONG < 64
    /** A lock for the last_overwritten ktime variable. Since ktime_t is 8b long,
     this is only needed on x32. */
    seqlock_t last_overwritten_lock;
#endif
    /** The timestamp of the last_overwritten variable on the buffer */
    ktime_t last_overwritten;
    /** A pointer to the array where the data is kept */
    char *data_array;
    /** A pointer to the array where the timestamps are kept */
    ktime_t *time_array;
    /** Double Linked list structure that pointers to the next/previous elements */
    struct hlist_node hlist_node;
    /** Reading/writing sequential lock */
    struct raven_buffer_lock lock;
    /** The id of this event buffer */
    key_t id;
    /** The Process ID of the writer */
    pid_t writer_pid;
    /** the event_buffers_length of the array, how many indexes exist */
    buffer_length_t length;
    /** the index last written by the writer */
    buffer_length_t index;
    /** how many bytes each index takes */
    unsigned short index_size;
};


/*
 * ===============================================
 *          x32 ioctl Compat Definitions
 * ===============================================
 */

/**
 * Represents a compatibility layer between a X64 kernel and an x32 application. This enables an x32 application to use event buffers.
 *
 * For this to be possible we must convert the x64 data structures to x32, so that the system can read them. For example, on x64 pointers are 8 bytes long, but on x32 pointers are 4 bytes long, thus we must use a compat_uptr_t instead of void *.
 */
#ifdef CONFIG_COMPAT

/**
 * Represents an x32 compatibility layer event buffer.
 */
struct compat_event_buffer {
    /** the id of the buffer */
    key_t id;
    /** the number of indexes of the buffer */
    unsigned int length;
    /** how many bytes each index takes */
    unsigned short index_size;
};

/**
 *  x32 compat information about a specific event buffer
 */
struct compat_event_buffer_info {
    /** the last written timestamp */
    struct compat_timespec last_written;
    /** the last overwritten timestamp*/
    struct compat_timespec last_overwritten;
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
 *  x32 compat information about a list of event buffers
 */
struct compat_event_buffer_list {
    /** An x32 compatibility pointer to an array of event_buffer_info that contains information about each event buffer, converts struct compat_event_buffer_info * */
    compat_uptr_t event_buffers;
    /** the event_buffers array size  */
    unsigned int event_buffers_length;
    /** how many event_buffers exist */
    unsigned int number_of_buffers;
};

/**
 * Represents an x32 compatibility event reader.
 *
 * There are two types of readers:
 *
 * - A synchronized reader uses a synchronization variable shared between buffers, events popped from these synchronized event readers are always larger than the last one popped in any reader. The synch variable points to the address of the shared time stamp.
 * - A regular reader is isolated from other buffers, events popped from this reader are always larger than the last one popped from this reader. The synch variable is NULL.
 *
 * The event_data length is not saved since the kernel land knows how big each index is.
 *
 * @warning Users should be careful modifying these variables as they can result in undefined behavior.
 */
struct compat_event_reader {
    /** the time of the read event */
    struct compat_timespec event_time;
    /** the time of the last read event */
    struct compat_timespec last_read;
    /** a pointer to the event data, converts void * */
    compat_uptr_t event_data;
    /** a pointer to the synchronization variable. NULL if this buffer isn't a synchronized reader, converts struct compat_timespec * */
    compat_uptr_t synch;
    /** the id of the buffer to pop events from */
    key_t id;
    /** the index that was last read */
    unsigned int index;
    /** if there is a gap in the trace. that is if elements have been overwritten without us reading them */
    unsigned char gap;
    /** if the buffer is empty */
    unsigned char empty;
};

/**
 * Represents an x32 compatibility event writer.
 *
 *  The data length is not saved since the kernel land knows how big each index is.
 */
struct compat_event_writer {
    /** A pointer to the data being saved, converts to void * */
    compat_uptr_t data;
    /** The id of the buffer to write events to */
    key_t buffer_id;
};

/** Compat IOCTL Command code to push an event to a buffer */
#define RAVEN_COMPAT_IOC_PUSH      _IOW(RAVEN_IOC_MAGIC, 0, compat_uptr_t)
/** IOCTL Command code to pop an event from a buffer */
#define RAVEN_COMPAT_IOC_POP       _IOWR(RAVEN_IOC_MAGIC, 1, compat_uptr_t)
/** IOCTL Command code to pop the newest event from a buffer */
#define RAVEN_COMPAT_IOC_POP_HEAD  _IOWR(RAVEN_IOC_MAGIC, 2, compat_uptr_t)
/** IOCTL Command code to peek an event from a buffer */
#define RAVEN_COMPAT_IOC_PEEK       _IOWR(RAVEN_IOC_MAGIC, 3, compat_uptr_t)
/** IOCTL Command code to peek the newest event from a buffer */
#define RAVEN_COMPAT_IOC_PEEK_HEAD  _IOWR(RAVEN_IOC_MAGIC, 4, compat_uptr_t)
/** IOCTL Command code to create a buffer */
#define RAVEN_COMPAT_IOC_CREATE    _IOWR(RAVEN_IOC_MAGIC, 5, compat_uptr_t)
/** IOCTL Command code to destroyBuffer a buffer */
#define RAVEN_COMPAT_IOC_DESTROY   _IOW(RAVEN_IOC_MAGIC, 6, key_t)
/** IOCTL Command code to change the buffers owner to the calling process */
#define RAVEN_COMPAT_IOC_ACQUIRE   _IOW(RAVEN_IOC_MAGIC, 7, key_t)
/** IOCTL Command code to get information about a buffer */
#define RAVEN_COMPAT_IOC_INFO      _IOWR(RAVEN_IOC_MAGIC, 8, compat_uptr_t)
/** IOCTL Command code to get information about all buffers */
#define RAVEN_COMPAT_IOC_LIST      _IOR(RAVEN_IOC_MAGIC, 9, compat_uptr_t)

#endif /* CONFIG_COMPAT */
#endif /* __KERNEL__ */
#endif //KERNEL_RAVEN_H
