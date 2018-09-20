#ifndef MONITOR_TYPES_H
#define MONITOR_TYPES_H


#if BITS_PER_LONG >= 64
    /**
     * Typedef to specify the length of the buffer, on X32 this value is an unsigned short and on X64 it is unsigned int.
     *  These limits are imposed because of locking restrictions as we also need to atomically save extra information with the index the writer is currently on.
     */
    typedef unsigned int length_t;
#else
    /**
    * Typedef to specify the length of the buffer, on X32 this value is an unsigned short and on X64 it is unsigned int.
    *  These limits are imposed because of locking restrictions as we also need to atomically save extra information with the index the writer is currently on.
    */
    typedef unsigned short length_t;
#endif

#endif //MONITOR_TYPES_H
