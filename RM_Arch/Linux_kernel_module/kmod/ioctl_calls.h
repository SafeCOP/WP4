/**
 * @file ioctl_calls.h
 *
 * This file contains declarations for IOCTL functions.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */

#ifndef KERNEL_IOCTL_CALLS_H
#define KERNEL_IOCTL_CALLS_H


#include <linux/errno.h>  /* error codes */
#include <linux/compiler.h> /* unlikely && likely */
#include <linux/hashtable.h> /* hash table */
#include <linux/seqlock.h> /* seqlock */
#include <linux/slab.h>   /* kmalloc() */
#include <linux/kernel.h> /* USHRT_MAX */
#include <linux/sched.h> /* struct task_struct for current->pid */
#include <asm/uaccess.h> /* copy_from_user && copy_to_user */

#include "raven.h"
#include "eventbuffer_operations.h"

/**
 * Peeks the newest event from a buffer.
 *
 * Reads the newest event from a buffer in an rcu enabled hash table without marking it as read.
 *
 *  Parameters are passed via an event_reader structure whose memory address is given by user space in arg. These parameters contain the event reader state, buffer information, and output data.
 *
 *  Accepts both synchronized and non synchronized buffers, the buffer
 *  is assumed to be synchronized unless the synch variable is NULL.
 *
 * @return 0 on success, error code if an error occurs.
 *
 * error codes:
 * - EFAULT : arg or event_data or synch reference an invalid memory region.
 * - EINVAL : if the buffer index is larger than the array event_buffers_length.
 * @ arg a memory address to the struct event_reader.
 * - ENOENT : invalid buffer ID
 */
long ioc_peek_head(const unsigned long arg) {
    struct event_reader pop_data;
    if (unlikely(copy_from_user((void *) &pop_data, (void *) arg, sizeof(struct event_reader))))
        return -EFAULT;

    char data[MAX_INDEX_SIZE];
    struct timespec tmp_reader_last_read = pop_data.last_read;
    buffer_length_t tmp_reader_index = (buffer_length_t) pop_data.index;

    struct raven_event_buffer *buf;
    rcu_read_lock();
    hash_for_each_possible_rcu(raven_hash_table, buf, hlist_node, pop_data.id)
    {
        if (buf->id == pop_data.id) {
            if (unlikely(pop_data.index >= buf->length)) {
                rcu_read_unlock();
                return -EINVAL;
            }

            if (pop_data.synch != NULL) {
                //synchronized buffer
                struct timespec time;
                if (unlikely(copy_from_user((void *) &time, (void *) pop_data.synch, sizeof(struct timespec)))) {
                    rcu_read_unlock();
                    return -EFAULT;
                }

                char popped = pop_head(buf, &tmp_reader_index, &pop_data.gap, &pop_data.empty, &tmp_reader_last_read,
                                       data, timespec_to_ktime(time));
                rcu_read_unlock();
                if (popped) {
                    if (unlikely(copy_to_user(pop_data.event_data, (void *) data, buf->index_size)))
                        return -EFAULT;
                    pop_data.event_time = tmp_reader_last_read;
                }
            } else {
                //regular buffer
                char popped = pop_head(buf, &tmp_reader_index, &pop_data.gap, &pop_data.empty, &tmp_reader_last_read,
                                       data, timespec_to_ktime(pop_data.last_read));
                rcu_read_unlock();
                if (popped) {
                    if (unlikely(copy_to_user(pop_data.event_data, (void *) data, buf->index_size)))
                        return -EFAULT;
                    pop_data.event_time = tmp_reader_last_read;
                }
            }

            if (unlikely(copy_to_user((void *) arg, (void *) &pop_data, sizeof(struct event_reader))))
                return -EFAULT;

            return 0;
        }
    }

    rcu_read_unlock();
    return -ENOENT;
}

/**
 * Pops the newest event from a buffer.
 *
 * Reads the newest event from a buffer in an rcu enabled hash table  marking it as read.
 *
 *  Parameters are passed via an event_reader structure whose memory address is given by user space in arg. These parameters contain the event reader state, buffer information, and output data.
 *
 *  Accepts both synchronized and non synchronized buffers, the buffer
 *  is assumed to be synchronized unless the synch variable is NULL.
 *
 * @return 0 on success, error code if an error occurs.
 *
 * error codes:
 * - EFAULT : arg or event_data or synch reference an invalid memory region.
 * - EINVAL : if the buffer index is larger than the array event_buffers_length.
 * @ arg a memory address to the struct event_reader.
 * - ENOENT : invalid buffer ID
 */
long ioc_pop_head(const unsigned long arg) {
    struct event_reader pop_data;
    if (unlikely(copy_from_user((void *) &pop_data, (void *) arg, sizeof(struct event_reader))))
        return -EFAULT;

    char data[MAX_INDEX_SIZE];
    buffer_length_t index = (buffer_length_t) pop_data.index;

    struct raven_event_buffer *buf;
    rcu_read_lock();
    hash_for_each_possible_rcu(raven_hash_table, buf, hlist_node, pop_data.id)
    {
        if (buf->id == pop_data.id) {
            if (unlikely(pop_data.index >= buf->length)) {
                rcu_read_unlock();
                return -EINVAL;
            }

            if (pop_data.synch != NULL) {
                //synchronized buffer
                struct timespec time;
                if (unlikely(copy_from_user((void *) &time, (void *) pop_data.synch, sizeof(struct timespec)))) {
                    rcu_read_unlock();
                    return -EFAULT;
                }

                char popped = pop_head(buf, &index, &pop_data.gap, &pop_data.empty, &pop_data.last_read, data,
                                       timespec_to_ktime(time));
                rcu_read_unlock();
                if (popped) {
                    pop_data.event_time = pop_data.last_read;

                    if (unlikely(copy_to_user(pop_data.event_data, (void *) data, buf->index_size)))
                        return -EFAULT;

                    if (unlikely(copy_to_user(pop_data.synch, (void *) &pop_data.last_read, sizeof(struct timespec))))
                        return -EFAULT;
                }
                pop_data.index = index;
            } else {
                //regular buffer
                char popped = pop_head(buf, &index, &pop_data.gap, &pop_data.empty, &pop_data.last_read, data,
                                       timespec_to_ktime(pop_data.last_read));
                rcu_read_unlock();
                if (popped) {
                    if (unlikely(copy_to_user(pop_data.event_data, (void *) data, buf->index_size)))
                        return -EFAULT;

                    pop_data.event_time = pop_data.last_read;
                    pop_data.index = index;
                }
            }

            if (unlikely(copy_to_user((void *) arg, (void *) &pop_data, sizeof(struct event_reader))))
                return -EFAULT;

            return 0;
        }
    }

    rcu_read_unlock();
    return -ENOENT;
}

/**
 * Peeks an event from a buffer.
 *
 * Reads the an event from a buffer in an rcu enabled hash table  without marking it as read.
 *
 *  Parameters are passed via an event_reader structure whose memory address is given by user space in arg. These parameters contain the event reader state, buffer information, and output data.
 *
 *  Accepts both synchronized and non synchronized buffers, the buffer
 *  is assumed to be synchronized unless the synch variable is NULL.
 *
 * @return 0 on success, error code if an error occurs.
 *
 * error codes:
 * - EFAULT : arg or event_data or synch reference an invalid memory region.
 * - EINVAL : if the buffer index is larger than the array event_buffers_length.
 * @ arg a memory address to the struct event_reader.
 * - ENOENT : invalid buffer ID
 */
long ioc_peek(const unsigned long arg) {
    struct event_reader pop_data;
    if (unlikely(copy_from_user((void *) &pop_data, (void *) arg, sizeof(struct event_reader))))
        return -EFAULT;

    char data[MAX_INDEX_SIZE];
    struct timespec tmp_last_read = pop_data.last_read;
    buffer_length_t tmp_reader_index = (buffer_length_t) pop_data.index;

    struct raven_event_buffer *buf;
    rcu_read_lock();
    hash_for_each_possible_rcu(raven_hash_table, buf, hlist_node, pop_data.id)
    {
        if (buf->id == pop_data.id) {
            if (unlikely(pop_data.index >= buf->length)) {
                rcu_read_unlock();
                return -EINVAL;
            }

            if (pop_data.synch != NULL) {
                //synchronized buffer
                struct timespec time;
                if (unlikely(copy_from_user((void *) &time, (void *) pop_data.synch, sizeof(struct timespec)))) {
                    rcu_read_unlock();
                    return -EFAULT;
                }

                char popped = pop_synch(buf, &tmp_reader_index, &pop_data.gap, &pop_data.empty, &tmp_last_read, data,
                                        timespec_to_ktime(time));
                rcu_read_unlock();
                if (popped) {
                    if (unlikely(copy_to_user(pop_data.event_data, (void *) data, buf->index_size)))
                        return -EFAULT;
                    pop_data.event_time = tmp_last_read;
                }
            } else {
                //regular buffer
                char popped = pop(buf, &tmp_reader_index, &pop_data.gap, &pop_data.empty, &tmp_last_read, data);
                rcu_read_unlock();
                if (popped) {
                    if (unlikely(copy_to_user(pop_data.event_data, (void *) data, buf->index_size)))
                        return -EFAULT;

                    pop_data.event_time = tmp_last_read;
                }
            }

            if (unlikely(copy_to_user((void *) arg, (void *) &pop_data, sizeof(struct event_reader))))
                return -EFAULT;

            return 0;
        }
    }

    rcu_read_unlock();
    return -ENOENT;
}

/**
 * Pops an event from a buffer.
 *
 * Reads the an event from a buffer in an rcu enabled hash table  marking it as read.
 *
 *  Parameters are passed via an event_reader structure whose memory address is given by user space in arg. These parameters contain the event reader state, buffer information, and output data.
 *
 *  Accepts both synchronized and non synchronized buffers, the buffer
 *  is assumed to be synchronized unless the synch variable is NULL.
 *
 * @return 0 on success, error code if an error occurs.
 *
 * error codes:
 * - EFAULT : arg or event_data or synch reference an invalid memory region.
 * - EINVAL : if the buffer index is larger than the array event_buffers_length.
 * @ arg a memory address to the struct event_reader.
 * - ENOENT : invalid buffer id
 */
long ioc_pop(const unsigned long arg) {
    struct event_reader pop_data;
    if (unlikely(copy_from_user((void *) &pop_data, (void *) arg, sizeof(struct event_reader))))
        return -EFAULT;

    char data[MAX_INDEX_SIZE];
    buffer_length_t index = (buffer_length_t) pop_data.index;

    struct raven_event_buffer *buf;
    rcu_read_lock();
    hash_for_each_possible_rcu(raven_hash_table, buf, hlist_node, pop_data.id)
    {
        if (buf->id == pop_data.id) {
            if (unlikely(pop_data.index >= buf->length)) {
                rcu_read_unlock();
                return -EINVAL;
            }

            if (pop_data.synch != NULL) {
                //synchronized buffer
                struct timespec time;
                if (unlikely(copy_from_user((void *) &time, (void *) pop_data.synch, sizeof(struct timespec)))) {
                    rcu_read_unlock();
                    return -EFAULT;
                }

                char popped = pop_synch(buf, &index, &pop_data.gap, &pop_data.empty, &pop_data.last_read, data,
                                        timespec_to_ktime(time));
                rcu_read_unlock();
                if (popped) {
                    if (unlikely(copy_to_user(pop_data.event_data, (void *) data, buf->index_size)))
                        return -EFAULT;

                    if (unlikely(copy_to_user((void *) pop_data.synch, (void *) &pop_data.last_read,
                                              sizeof(struct timespec))))
                        return -EFAULT;

                    pop_data.event_time = pop_data.last_read;
                }
                pop_data.index = index;
            } else {
                //regular buffer
                char popped = pop(buf, &index, &pop_data.gap, &pop_data.empty, &pop_data.last_read, data);
                rcu_read_unlock();
                if (popped) {
                    pop_data.event_time = pop_data.last_read;
                    pop_data.index = index;
                    if (unlikely(copy_to_user(pop_data.event_data, (void *) data, buf->index_size)))
                        return -EFAULT;
                }
            }

            if (unlikely(copy_to_user((void *) arg, (void *) &pop_data, sizeof(struct event_reader))))
                return -EFAULT;

            return 0;
        }
    }

    rcu_read_unlock();
    return -ENOENT;
}

/**
 * Pushes an event to a buffer in O(1) constant time.
 *
 * Pushes an event to an event buffer inside a rcu enabled hash table in O(1) constant time, the event time is taken from the MONOTONIC clock. Only the buffers owner can push events to a buffer.
 *
 * Parameters are passed via an event_writer structure whose memory address is given by user space in arg. These parameters contain the event data and the buffer id.
 *
 * @warning push is not synchronized internally, that means if you want multiple processes/threads pushing into the same buffer then they must be synchronized externally.
 *
 *  @return 0 on success, error code if an error occurs.
 *
 * error codes:
 * - EFAULT : arg or event_data references an invalid memory region.
 * - ENOENT : invalid bufer id.
 * - EACCES : The buffer isn't owned by the calling process.
 *
 */
long ioc_push(const unsigned long arg) {
    struct event_writer push_data;
    if (unlikely(copy_from_user((void *) &push_data, (void *) arg, sizeof(struct event_writer))))
        return -EFAULT;

    char event_data[MAX_INDEX_SIZE];

    struct raven_event_buffer *buf;
    rcu_read_lock();
    hash_for_each_possible_rcu(raven_hash_table, buf, hlist_node, push_data.buffer_id)
    {
        if (push_data.buffer_id == buf->id) {
            if (buf->writer_pid != current->pid) {
                rcu_read_unlock();
                return -EACCES;
            }
            if (unlikely(copy_from_user((void *) &event_data, push_data.data, buf->index_size))) {
                rcu_read_unlock();
                return -EFAULT;
            }

            push(buf, event_data);
            rcu_read_unlock();
            return 0;
        }
    }

    rcu_read_unlock();
    return -ENOENT;
}

/**
 * Destroys a buffer.
 *
 * Removes a buffer from an rcu enabled hash table, and frees all associated resources with it.
 *
 * This method is synchronized internally, thus multiple processes/threads can call this function at the same time. Only the buffers owner can destroyBuffer the buffer.
 *
 * @arg the id of the buffer to remove
 *
 *  @return 0 on success, error code if an error occurs.
 *
 * error codes:
 * - ENOENT : invalid buffer id
 * - EACCES : The buffer isn't owned by the calling process
 */
long ioc_destroy(const unsigned long arg) {
    key_t id = (key_t) arg;
    struct raven_event_buffer *buf;
    mutex_lock(&raven_mutex);
    hash_for_each_possible_rcu(raven_hash_table, buf, hlist_node, id)
    {
        if (id == buf->id) {
            if (buf->writer_pid != current->pid) {
                mutex_unlock(&raven_mutex);
                return -EACCES;
            }
            hash_del_rcu(&buf->hlist_node);
            WRITE_ONCE(number_of_buffers, --number_of_buffers);
            mutex_unlock(&raven_mutex);
            synchronize_rcu();
            kfree(buf->data_array);
            kfree(buf->time_array);
            kfree(buf);
            return 0;
        }
    }

    mutex_unlock(&raven_mutex);
    return -ENOENT;
}

/**
 * Acquires the buffer.
 *
 * Changes the buffers owner to the calling process.
 *
 * @arg arg the id of the buffer to acquire
 *
 * @return 0 on success, error code if an error occurs.
 *
 * error codes:
 * - ENOENT : invalid buffer id
 */
long ioc_acquire(const unsigned long arg) {
    key_t id = (key_t) arg;
    struct raven_event_buffer *buf;
    rcu_read_lock();
    hash_for_each_possible_rcu(raven_hash_table, buf, hlist_node, id)
    {
        if (id == buf->id) {
            WRITE_ONCE(buf->writer_pid, current->pid);
            rcu_read_unlock();
            return 0;
        }
    }

    rcu_read_unlock();
    return -ENOENT;
}

/**
 * Creates an event buffer.
 *
 * Creates an event buffer and inserts it into an rcu enabled hash table.
 *
 * Parameters are passed via an event_buffer structure whose memory address is given by user space in arg. These parameters contain the buffer id , the array event_buffers_length (length), and how many bytes each index takes.
 *
 * This method is synchronized internally, thus multiple processes/threads can call this function at the same time.
 *
 * The array event_buffers_length is invalid if it is equal to zero or larger than MAX_ARRAY_SIZE, defined as 1936b on 64 bit platforms and 948b on x32 platforms
 *
 *  @return 0 on success, error code if an error occurs.
 *
 * error codes:
 * - EFAULT : arg references an invalid memory region
 * - EINVAL : the buffer event_buffers_length is invalid, it is either zero or larger than what your architecture supports.
 * - EEXIST : a buffer already exists this ID
 * - ENOMEM : There is not enough free memory to allocate the buffer
 */
long ioc_create(const unsigned long arg) {
    struct event_buffer event_buffer;
    if (unlikely(copy_from_user((void *) &event_buffer, (void *) arg, sizeof(struct event_buffer))))
        return -EFAULT;

    if (event_buffer.index_size == 0 || event_buffer.index_size > MAX_INDEX_SIZE || event_buffer.length == 0)
        return -EINVAL;

#if BITS_PER_LONG < 64
    if (event_buffer.length > USHRT_MAX)
        return -EINVAL;
#endif

    struct raven_event_buffer *buf;
    mutex_lock(&raven_mutex);
    hash_for_each_possible_rcu(raven_hash_table, buf, hlist_node, event_buffer.id)
    {
        if (event_buffer.id == buf->id) {
            mutex_unlock(&raven_mutex);
            return -EEXIST;
        }
    }

    buf = kmalloc(sizeof(struct raven_event_buffer), GFP_KERNEL);
    if (unlikely(buf == NULL))
        goto fail_buf_alloc;
    buf->time_array = kzalloc(event_buffer.length * sizeof(ktime_t), GFP_KERNEL);
    if (unlikely(buf->time_array == NULL))
        goto fail_time_alloc;
    buf->data_array = kmalloc(event_buffer.length * event_buffer.index_size, GFP_KERNEL);
    if (unlikely(buf->data_array == NULL))
        goto fail_data_alloc;


    buf->last_overwritten = ktime_set(0, 0);
#if BITS_PER_LONG < 64
    seqlock_init(&buf->last_overwritten_lock);
#endif
    array_seqlock_initialize(&buf->lock);
    buf->length = (buffer_length_t) event_buffer.length;
    buf->index_size = event_buffer.index_size;
    buf->index = 0;
    buf->id = event_buffer.id;
    buf->writer_pid = current->pid;

    hash_add_rcu(raven_hash_table, &buf->hlist_node, buf->id);
    WRITE_ONCE(number_of_buffers, ++number_of_buffers);
    mutex_unlock(&raven_mutex);
    return 0;

    fail_data_alloc:
    kfree(buf->time_array);
    fail_time_alloc:
    kfree(buf);
    fail_buf_alloc:
    mutex_unlock(&raven_mutex);
    return -ENOMEM;
}

/**
 * Gets information about an event buffer given its ID.
 *
 * Fills a struct event_buffer_info with information on an event buffer given its ID.
 *
 * Parameters are passed via an event_buffer_info structure whose memory address is given by user space in arg. These parameters contain the buffer id , the array event_buffers_length (length), and how many bytes each index takes, the last written timestamp, the last overwritten timestamp, and the pid of the owning process.
 *
 *  @return 0 on success, error code if an error occurs.
 *
 * error codes:
 * - EFAULT : arg references an invalid memory region
 * - ENOENT : invalid buffer id
 */
long ioc_info(const unsigned long arg) {
    struct event_buffer_info event_buffer;
    if (unlikely(copy_from_user((void *) &event_buffer, (void *) arg, sizeof(struct event_buffer_info))))
        return -EFAULT;

    struct raven_event_buffer *buf;
    rcu_read_lock();
    hash_for_each_possible_rcu(raven_hash_table, buf, hlist_node, event_buffer.id)
    {
        if (event_buffer.id == buf->id) {
            event_buffer.length = buf->length;
            event_buffer.index_size = buf->index_size;
            event_buffer.owner = buf->writer_pid;
            ktime_t last_overwritten;
            readLastOverwritten(buf, &last_overwritten);
            ktime_t last_written;
            readTime(buf, buf->index, &last_written);

            rcu_read_unlock();

            event_buffer.last_overwritten = ktime_to_timespec(last_overwritten);
            event_buffer.last_written = ktime_to_timespec(last_written);

            if (unlikely(copy_to_user((void *) arg, (void *) &event_buffer, sizeof(struct event_buffer_info)))) {
                return -EFAULT;
            }

            return 0;
        }
    }

    rcu_read_unlock();
    return -ENOENT;
}

/**
 * Gets information about all event buffers.
 *
 * Fills a struct event_buffer_list with information on all event buffers as long as the allocated array event_buffers can fit them, the number of event buffers is always written to number_of_buffers.
 *
 * For each buffer, the following is listed:
 * - the buffer id
 * - the buffer event_buffers_length or length
 * - the event_buffers_length of each index in bytes
 * - the last written time stamp
 * - the last overwritten time stamp
 * - the pid of the owners process
 *
 * Parameters are passed via an event_buffer_info structure whose memory address is given by user space in arg. These parameters contain an array of type struct event_buffer_info where the buffer information will be set named event_buffers, the event_buffers_length of this array named event_buffers_length, and the a variable to keep track of how many event buffers exist named number_of_buffers.
 *
 *  @return 0 on success, error code if an error occurs.
 *
 * error codes:
 * - EFAULT : arg or event_buffers reference an invalid memory region
 */
long ioc_list(const unsigned long arg) {
    struct event_buffer_list list;
    if (unlikely(copy_from_user((void *) &list, (void *) arg, sizeof(struct event_buffer_list))))
        return -EFAULT;

    if (list.event_buffers_length > 0) {
        list.number_of_buffers = 0;
        struct event_buffer_info *event_buffers = list.event_buffers;

        size_t cursor;
        struct raven_event_buffer *buf;
        rcu_read_lock();
        hash_for_each_rcu(raven_hash_table, cursor, buf, hlist_node)
        {
            struct event_buffer_info tmp;
            tmp.id = buf->id;
            tmp.index_size = buf->index_size;
            tmp.length = buf->length;
            tmp.owner = buf->writer_pid;
            ktime_t last_overwritten;
            readLastOverwritten(buf, &last_overwritten);
            ktime_t last_written;
            readTime(buf, buf->index, &last_written);

            tmp.last_overwritten = ktime_to_timespec(last_overwritten);
            tmp.last_written = ktime_to_timespec(last_written);

            if (unlikely(copy_to_user((void *) event_buffers, (void *) &tmp, sizeof(struct event_buffer_info)))) {
                rcu_read_unlock();
                return -EFAULT;
            }

            if (++list.number_of_buffers < list.event_buffers_length)
                ++event_buffers;
            else
                goto out;
        }
        out:
        rcu_read_unlock();
    }

    list.number_of_buffers = READ_ONCE(number_of_buffers);

    if (unlikely(copy_to_user((void *) arg, (void *) &list, sizeof(struct event_buffer_list))))
        return -EFAULT;

    return 0;
}

/**
 * IOCTL entry point for the raven module.
 *
 * This is the ioctl entry point function for the raven module. The function does what it can to ensure the request is valid and it sub functions validate all input data.
 *
 * The following operations are supported:
 * - push : push an event to a buffer
 * - pop : pop an event from a buffer
 * - pop head : pop the newest element from a buffer
 * - peek : peeks an element from a buffer
 * - peek head :  peeks the newest element from a buffer
 * - create :  creates a new event buffer
 * - destroyBuffer : destroys an event buffer
 * - list : lists all event buffers
 * - info : gives information about an event buffer
 *
 * @param f not used
 * @param cmd the requested command
 * @param arg the argument for this command
 *
 * @return 0 on success, error code if an error occurs
 *
 * error codes:
 * - EFAULT : If arg is a pointer, then arg references an invalid memory region
 * - ENOTTY : The specified request does not apply to the kind of object that the descriptor d references.
 *
 * other error codes might be thrown, please check the documentation on the functions responsible for the operations above
 */
long raven_ioctl(struct file *f, unsigned int cmd, unsigned long arg) {
    /*
	 * extract the type and number bitfields, and don't decode
	 * wrong cmds: return ENOTTY (inappropriate ioctl) before access_ok()
	 */
    if (unlikely(_IOC_TYPE(cmd) != RAVEN_IOC_MAGIC)) return -ENOTTY;
    if (unlikely(_IOC_NR(cmd) > RAVEN_IOC_MAXNR)) return -ENOTTY;

    /*
	 * the direction is a bitmask, and VERIFY_WRITE catches R/W
	 * transfers. `Type' is user-oriented, while
	 * access_ok is kernel-oriented, so the concept of "read" and
	 * "write" is reversed
     */
    if (_IOC_DIR(cmd) & _IOC_READ) {
        if (unlikely(!access_ok(VERIFY_WRITE, (void __user *)arg, _IOC_SIZE(cmd)))) return -EFAULT;
    } else if (_IOC_DIR(cmd) & _IOC_WRITE) {
        if (unlikely(!access_ok(VERIFY_READ, (void __user *)arg, _IOC_SIZE(cmd)))) return -EFAULT;
    }

    switch (cmd) {
        case RAVEN_IOC_PUSH:
            return ioc_push(arg);

        case RAVEN_IOC_POP:
            return ioc_pop(arg);

        case RAVEN_IOC_POP_HEAD:
            return ioc_pop_head(arg);

        case RAVEN_IOC_PEEK:
            return ioc_peek(arg);

        case RAVEN_IOC_PEEK_HEAD:
            return ioc_peek_head(arg);

        case RAVEN_IOC_CREATE:
            return ioc_create(arg);

        case RAVEN_IOC_DESTROY:
            return ioc_destroy(arg);

        case RAVEN_IOC_ACQUIRE:
            return ioc_acquire(arg);

        case RAVEN_IOC_LIST:
            return ioc_list(arg);

        case RAVEN_IOC_INFO:
            return ioc_info(arg);
        default:  /* not redundant as _IOC_SIZE could still be wrong */
            return -ENOTTY;
    }
}

#endif //KERNEL_IOCTL_CALLS_H
