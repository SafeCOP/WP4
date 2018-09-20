/**
 * @file raven_api.h
 *
 * This file contains all the raven API function declarations such as raven_init, raven_pop, and raven_push.
 *
 * @see raven_api.c
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */

#ifndef KERNEL_RAVEN_API_H
#define KERNEL_RAVEN_API_H


#include "api_types.h"

/**
 * Initializes the module. Must be called before any other operation.
 *
 * This opens a file descriptor to the module device interface at /dev/raven, storing
 * the file descriptor ID in a static variable.
 *
 * @warning Must be called only ONCE for each process and is not thread safe.
 *
 * @return returns the new file descriptor or -1 if an error occurred (in which case, errno is set appropriately).
 *
 * Please consult your linux manual on the errno codes open might produce.
 */
extern int raven_init();

/**
 * Terminates the module by freeing all associated resources.
 *
 * This closes the file descriptor to the module interface at /dev/raven.
 *
 * @return returns 0 on success. On error, -1 is returned, and errno is set appropriately. Please consult your linux
 * manual on the errno codes close might produce.
 */
extern int raven_term();

/**
 * Lists the current open buffers at buffer_list.
 *
 * Lists all the open buffers via an ioctl call to /dev/raven, detailing:
 *   - the buffer id
 *   - the buffer length
 *   - the data block length
 *   - the last written timespec
 *   - the last overwritten timespec
 *   - the pid of the process who owns this buffer
 *
 *   The data is stored in an array whose length and address is stored at buffer_list.
 *   If the array is too small to contain all the buffers, the array is filled  and the number of buffers is
 *   reported via the number_of_buffers variable in buffer_list.
 *
 * @param buffer_list a pointer to a event_buffer_list structure.
 * @param event_buffer_array an array of struct event_buffer_info to store information about the buffers.
 * @param array_length the length of the event_buffer_array.
 *
 * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
 *
 * errno error list:
 *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
 *  - EFAULT : buffer_list or an address within it references an inaccessible memory area.
 *  - EINVAL : Request or buffer_list is not valid. Never thrown inside the module, likely you forgot to initialize
 *  via raven_init().
 *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
 *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
 *
 *  Please consult your linux manual to see what errno codes ioctl might produce.
 */
extern int raven_list_buffers(struct event_buffer_list *buffer_list, struct event_buffer_info *event_buffer_array,
                              size_t array_length);

/**
 * Provides information about a specific Buffer.
 *
 * Provides the following information about a buffer via an ioctl call to /dev/raven:
 *   - the buffer id
 *   - the buffer length
 *   - the data block length
 *   - the last written timespec
 *   - the last overwritten timespec
 *   - the pid of the process who owns this buffer
 *
 *   The information will be stored in the address pointed by event_buffer.
 *   @param buffer_id the id of the buffer
 *   @param event_buffer A pointer to the event_buffer_info data structure used to store the data.
 *
 *   @return returns 0 on success. On error -1 is returned and errno is set appropriately.
 *
 * errno error list:
 *  - ENOENT : The buffer id at event_buffer does not exist.
 *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
 *  - EFAULT : buffer_list references an inaccessible memory area.
 *  - EINVAL : Request or buffer_list is not valid. Never thrown inside the module, likely you forgot to initialize via raven_init().
 *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
 *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
 *
 *  Please consult your linux manual to see what errno codes ioctl might produce.
 */
extern int raven_buffer_info(key_t buffer_id, struct event_buffer_info *event_buffer);

/**
 * Creates an event buffer.
 *
 * Creates an event buffer of ID id, data block event_buffers_length index_size and length length via an ioctl to /dev/raven. Create is synchronized internally.
 *
 * @param id the id of the buffer to create.
 * @param index_size the data block event_buffers_length of the buffer to create.
 * @param length the array event_buffers_length of the buffer to create.
 *
 *
 * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
 *
 * errno error list:
 *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
 *  - EFAULT : arg references an inaccessible memory area.
 *  - EEXIST : A buffer with an ID id already exists.
 *  - EINVAL : Request or args are invalid. The array event_buffers_length is invalid if it is equal
 *  to zero or larger than MAX_ARRAY_SIZE, defined as 1936b on 64 bit platforms and 948b on x32 platforms. It is also possible that you forgot to initialize the module with raven_init().
 *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
 *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
 *  - ENOMEM : Not enough memory is available to allocate the buffer.
 *
 *  Please consult your linux manual to see what errno codes ioctl might produce.
 */
extern int raven_buffer_create(key_t id, unsigned int length, unsigned short index_size);

/**
 * Destroys an event buffer of ID id.
 *
 * Destroys an  event buffer of ID id via an ioctl call to /dev/raven. Destroy is synchronized internally. Only the buffers owner can destroy the buffer.
 *
 * @param id the of the buffer to destroyBuffer.
 *
 * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
 *
 * errno error list:
 *  - ENOENT : No buffer with ID id exists.
 *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
 *  - EINVAL : Request is not valid. Never thrown inside the module, likely you forgot to initialize via raven_init().
 *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
 *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
 *  - EACCES : The buffer isn't owned by the calling process
 *
 *  Please consult your linux manual to see what errno codes ioctl might produce.
 */
extern int raven_buffer_destroy(key_t id);

/**
 * Pushes an event to an event buffer in O(1) constant time.
 *
 * Pushes the data to the event_buffer whose id matches the id in buffer_push.buffer_id via an ioctl call to /dev/raven.
 *
 * The event timestamp is generated from clock monotonic inside the kernel call. Only the buffers owner can push events to a buffer.
 *
 * @param buffer_push A pointer to the data structure event_writer that contains the buffer id and a pointer to the
 * data to be pushed.
 *
 * @warning push is not synchronized internally, that means if you want multiple processes/threads pushing into the same buffer then they must be synchronized externally.
 *
 * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
 *
 * errno error list:
 *  - ENOENT : No buffer with ID id exists.
 *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
 *  - EFAULT : buffer_push or the data address inside it references an inaccessible memory area.
 *  - EINVAL : Request is not valid. Never thrown inside the module, likely you forgot to initialize via raven_init().
 *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
 *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
 *  - EACCES : The buffer isn't owned by the calling process.
 *
 *  Please consult your linux manual to see what errno codes ioctl might produce.
 */
extern int raven_push(struct event_writer *buffer_push);

/**
 * Pops an event from an event buffer.
 *
 * Pops an event from the buffer whose id matches the one in the event_reader structure pointed by buffer_pop via an ioctl call to /dev/raven.
 *
 * The output will be done via the buffer_pop structure:
 *  - If the buffer is empty then the empty field under buffer_pop will be set to 1, otherwise it will be set to 0.
 *  - If there is a gap in the trace, that is if elements have been overwritten before reading them the gap field under buffer_pop will be set to 1, otherwise it will be set to 0.
 *  - The popped event time will be set to the event_time field.
 *  - The popped event data will be copied to the event_data.
 *  - The last read event from this Buffer is on the last_read field.
 *  - If this event reader is synchronized, then the synch field on buffer_pop references the synchronized timespec variable.
 *
 * @warning Users should be careful not to modify the event_reader structure as it can result in undefined behavior.
 *
 * @param buffer_pop a pointer to an event_reader structure configured by raven_init_pop.
 *
 * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
 *
 * errno error list:
 *  - ENOENT : No buffer with ID id exists.
 *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
 *  - EFAULT : buffer_pop or some address inside it references an inaccessible memory area.
 *  - EINVAL : Request is not valid. This could occur if the index is larger than the buffer event_buffers_length. It is also possible you forgot to initialize via raven_init().
 *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
 *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
 *
 *  Please consult your linux manual to see what errno codes ioctl might produce.
 *
 */
extern int raven_pop(struct event_reader *buffer_pop);

/**
 * Pops the newest event from an event buffer.
 *
 * Pops the newest event event from the buffer whose id matches the one in the event_reader structure pointed by buffer_pop via an ioctl call to /dev/raven.
 *
 * The output will be done via the buffer_pop structure:
 *  - If the buffer is empty then the empty field under buffer_pop will be set to 1, otherwise it will be set to 0.
 *  - If there is a gap in the trace, that is if elements have been overwritten before reading them the gap field under buffer_pop will be set to 1, otherwise it will be set to 0.
 *  - The popped event time will be set to the event_time field.
 *  - The popped event data will be copied to the event_data.
 *  - The last read event from this Buffer is on the last_read field.
 *  - If this event reader is synchronized, then the synch field on buffer_pop references the synchronized timespec variable.
 *
 * @warning Users should be careful not to modify the event_reader structure as it can result in undefined behavior.
 *
 * @param buffer_pop a pointer to an event_reader structure configured by raven_init_pop.
 *
 * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
 *
 * errno error list:
 *  - ENOENT : No buffer with ID id exists.
 *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
 *  - EFAULT : buffer_pop or some address inside it references an inaccessible memory area.
 *  - EINVAL : Request is not valid. This could occur if the index is larger than the buffer event_buffers_length. It is also possible you forgot to initialize via raven_init().
 *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
 *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
 *
 *  Please consult your linux manual to see what errno codes ioctl might produce.
 *
 */
extern int raven_pop_head(struct event_reader *buffer_pop);

/**
 * Peeks, reads an event from an event buffer without marking it as read.
 *
 * Reads an event from the buffer without marking it as read, whose id matches the one in the event_reader structure pointed by buffer_pop via an ioctl call to /dev/raven.
 *
 * The output will be done via the buffer_pop structure:
 *  - If the buffer is empty then the empty field under buffer_pop will be set to 1, otherwise it will be set to 0.
 *  - If there is a gap in the trace, that is if elements have been overwritten before reading them the gap field under buffer_pop will be set to 1, otherwise it will be set to 0.
 *  - The popped event time will be set to the event_time field.
 *  - The popped event data will be copied to the event_data.
 *  - The last read event from this Buffer is on the last_read field.
 *  - If this event reader is synchronized, then the synch field on buffer_pop references the synchronized timespec variable.
 *
 * @warning Users should be careful not to modify the event_reader structure as it can result in undefined behavior.
 *
 * @param buffer_pop a pointer to an event_reader structure configured by raven_init_pop.
 *
 * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
 *
 * errno error list:
 *  - ENOENT : No buffer with ID id exists.
 *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
 *  - EFAULT : buffer_pop or some address inside it references an inaccessible memory area.
 *  - EINVAL : Request is not valid. This could occur if the index is larger than the buffer event_buffers_length. It is also possible you forgot to initialize via raven_init().
 *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
 *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
 *
 *  Please consult your linux manual to see what errno codes ioctl might produce.
 *
 */
extern int raven_peek(struct event_reader *buffer_pop);

/**
 * Peeks, reads the newest event from an event buffer without marking it as read.
 *
 * Reads the newest event without marking it as read, from the buffer whose id matches the one in the event_reader structure pointed by buffer_pop, via an ioctl call to /dev/raven.
 *
 * The output will be done via the buffer_pop structure:
 *  - If the buffer is empty then the empty field under buffer_pop will be set to 1, otherwise it will be set to 0.
 *  - If there is a gap in the trace, that is if elements have been overwritten before reading them the gap field under buffer_pop will be set to 1, otherwise it will be set to 0.
 *  - The popped event time will be set to the event_time field.
 *  - The popped event data will be copied to the event_data.
 *  - The last read event from this Buffer is on the last_read field.
 *  - If this event reader is synchronized, then the synch field on buffer_pop references the synchronized timespec variable.
 *
 * @warning Users should be careful not to modify the event_reader structure as it can result in undefined behavior.
 *
 * @param buffer_pop a pointer to an event_reader structure configured by raven_init_pop.
 *
 * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
 *
 * errno error list:
 *  - ENOENT : No buffer with ID id exists.
 *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
 *  - EFAULT : buffer_pop or some address inside it references an inaccessible memory area.
 *  - EINVAL : Request is not valid. This could occur if the index is larger than the buffer event_buffers_length. It is also possible you forgot to initialize via raven_init().
 *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
 *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init().
 *
 *  Please consult your linux manual to see what errno codes ioctl might produce.
 *
 */
extern int raven_peek_head(struct event_reader *buffer_pop);

/**
 * Acquires the Buffer.
 *
 * Changes the buffers owner to the calling process.
 *
 * @arg buffer_id the id of the buffer to acquire
 *
 * @return returns 0 on success. On error -1 is returned and errno is set appropriately.
 *
 * errno error list:
 *  - ENOENT : No buffer with ID id exists.
 *  - EBADF  : fd is not a valid descriptor. Probably you forgot to initialize the module via raven_init().
 *  - EINVAL : Request is not valid. Never thrown inside the module, likely you forgot to initialize via raven_init().
 *  - ENOTTY : fd is not associated with a character special device. Likely you to initialize the module with raven_init().
 *  - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references. Likely you forgot to initialize the module with raven_init(). *
 *  Please consult your linux manual to see what errno codes ioctl might produce.
 */
extern int raven_acquire(key_t buffer_id);

/**
 * Initializes an event reader.
 *
 * Initializes an event reader to read events from a buffer. The event reader can be synchronized or not depending whether the synch parameter is null or not.
 *
 * A synchronized event reader shares the last read timestamp with other event readers, elements removed from these buffers will always be larger than the synchronization timestamp.
 *
 * @warning the synch variable is not initialized. You must zero or initialize the synchronization variable externally.
 *
 * @param pop An address to an event_reader structure to initialize.
 * @param id The id of the buffer to initialize the event_reader with, the event reader will read events from this buffer.
 * @param data A pointer to where the data read from this buffer will be saved from.
 * @param synch null if you do not this event reader to be synchronized, or a pointer to a timespec structure where the synchronization variable is kept.
 */
extern void raven_init_event_reader(struct event_reader *pop, key_t id, void *data, struct timespec *synch);

/**
 * Initializes an event writer to write events to a buffer.
 *
 * @arg writer a pointer to the event_writer structure to configure.
 * @arg buffer_id the id of the buffer to push events to
 * @arg data a pointer to the data which will be pushed to the buffer
 */
extern void raven_init_event_writer(struct event_writer *writer, key_t buffer_id, void *data);

/** Gets the event reader gap flag */
#define get_event_reader_gap(EVENT_READER) EVENT_READER.gap
/** Gets the event reader empty flag */
#define get_event_reader_empty(EVENT_READER) EVENT_READER.empty
/** Gets the id of the buffer this event reader is associated with */
#define get_event_reader_buffer_id(EVENT_READER) EVENT_READER.id
/** Gets the synchronization variable this event reader is associated with */
#define get_event_reader_synch(EVENT_READER) EVENT_READER.synch
/** Gets the event reader data address */
#define get_event_reader_event_data(EVENT_READER) EVENT_READER.event_data
/** Gets the timestamp of the read event */
#define get_event_reader_event_timestamp(EVENT_READER) EVENT_READER.event_time
/** Gets the timestamp of the last event marked as read */
#define get_event_reader_last_read(EVENT_READER) EVENT_READER.last_read

/** Gets the id of the buffer this event writer is associated with  */
#define get_event_writer_buffer_id(EVENT_WRITER) EVENT_WRITER.buffer_id
/** Gets the data address where data is read from this event_writer */
#define get_event_writer_data(EVENT_WRITER) EVENT_WRITER.data

/** Gets the time stamp of the last written event */
#define get_event_buffer_info_last_written(BUFFER_INFO) BUFFER_INFO.last_written
/** Gets the time stamp of the last overwritten event*/
#define get_event_buffer_info_last_overwritten(BUFFER_INFO) BUFFER_INFO.last_overwritten
/** Gets the event buffers id */
#define get_event_buffer_info_id(BUFFER_INFO) BUFFER_INFO.id
/** Gets the process ID of the process who owns the buffer  */
#define get_event_buffer_info_owner(BUFFER_INFO) BUFFER_INFO.owner
/** Gets the array length of this buffer */
#define get_event_buffer_info_length(BUFFER_INFO) BUFFER_INFO.length
/** Gets the event_buffers_length of each index on the buffer */
#define get_event_buffer_info_index_size(BUFFER_INFO) BUFFER_INFO.index_size

/** Gets the array containing information about each buffer  */
#define get_event_buffer_list_array(BUFFER_LIST) BUFFER_LIST.event_buffers
/** Gets the event_buffers_length of the array */
#define get_event_buffer_list_size(BUFFER_LIST) BUFFER_LIST.size
/** Gets how many event buffers exist on the kernel side */
#define get_event_buffer_list_number_of_buffers(BUFFER_LIST) BUFFER_LIST.size


#endif //KERNEL_RAVEN_API_H
