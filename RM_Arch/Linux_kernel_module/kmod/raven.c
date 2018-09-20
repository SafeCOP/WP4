/**
 * @file compat_ioctl_calls.h
 *
 * Main kernel module file, where module initialization, and de-initialization functions are kept.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */


#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h> /* device drivers */
#include <linux/miscdevice.h> /* misc_register && misc_deregister && misdevice*/
#include <linux/hashtable.h> /* hash table */

#include "ioctl_calls.h"

#ifdef CONFIG_COMPAT

#include "compat_ioctl_calls.h"

#endif
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Humberto Carvalho (1120409@isep.ipp.pt)");
MODULE_DESCRIPTION("A monitoring framework");
MODULE_VERSION("0.1");
MODULE_ALIAS(MODULE_NAME);

/** The hash table where the event buffers are kept */
DEFINE_HASHTABLE(raven_hash_table, RAVEN_HASH_TABLE_SIZE);

/** Number of buffers in the hash table */
size_t number_of_buffers = 0;

/** Mutual exclusion mutex, used for atomically creating/deleting buffers */
DEFINE_MUTEX(raven_mutex);

/**
 * Defines /dev/raven file operations.
 *
 * We only make use of ioctl and in case the x32 compatibility layer is
 * the compatibility ioctl.
 */
static struct file_operations raven_fops = {
        .owner =            THIS_MODULE,
        .unlocked_ioctl =   raven_ioctl,
#ifdef CONFIG_COMPAT
        .compat_ioctl =     raven_compat_ioctl,
#endif
};

/**
 * Describes the misc device /dev/raven.
 */
static struct miscdevice raven_misc = {
        .minor = MISC_DYNAMIC_MINOR,
        .name  = MODULE_NAME,
        .fops  = &raven_fops,
};


/**
 * Module init function, called during module loading.
 *
 * Only registers a misc device under /dev/raven.
 */
static int __init raven_init(void) {
    /*
     * Attempt to register the module as a misc. device with the
     * kernel.
     */
    return misc_register(&raven_misc);
}


/**
 * Module exit function, called during module unload.
 *
 * To unload the module, all buffers and their associated resources must be freed.
 *
 */
static void __exit raven_exit(void) {
    misc_deregister(&raven_misc);

    size_t cursor;
    struct raven_event_buffer *buf;
    hash_for_each_rcu(raven_hash_table, cursor, buf, hlist_node)
    {
        hash_del_rcu(&buf->hlist_node);
        kfree(buf->data_array);
        kfree(buf->time_array);
        kfree(buf);
    }
}

module_init(raven_init);
module_exit(raven_exit);
