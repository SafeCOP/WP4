#ifndef KERNEL_FACTORY_H_H
#define KERNEL_FACTORY_H_H

#include <iostream>
#include <errno.h>
#include "../EventBuffer.h"
#include "../../c/raven_api.h"

/**
 * A Factory Super class for the raven module.
 *
 * This factory only provides the initialize, terminate, getBufferID methods which are common to all Factories.
 *
 * @see EventBufferFactory
 * @see EventReaderFactory
 * @see EventWriterFactory
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
class Factory {
public: //todo buffer_list
    /**
     * Initializes the module. Must be called before any other operation.
     *
     * This opens a file descriptor to the module device interface at /dev/raven, storing
     * the file descriptor ID in a static variable.
     *
     * @warning Must be called only ONCE for each process, from only ONE of the factories.
     *
     * @warning not thread safe.
     *
     * @return returns the new file descriptor or -1 if an error occurred (in which case, errno is set appropriately).
     *
     * Please consult your linux manual on the errno codes open might produce.
     */
    static int initialize();

    /**
     * Terminates the module by freeing all associated resources.
     *
     * This closes the file descriptor to the module interface at /dev/raven.
     *
     * @return returns 0 on success. On error, -1 is returned, and errno is set appropriately. Please consult your linux
     * manual on the errno codes close might produce.
     */
    static int terminate();

    /**
     * Generates a buffer id from ftok.
     *
     * @arg file The file to generate the buffer id from.
     * @arg id the id to combine with the file. Only the low-order 8-bits of id are significant.
     *
     *  @return returns 0 on success. On error -1 is returned and errno is set appropriately.
     * Please consult your linux manual to see what errno codes ioctl might produce.
     */
    static key_t getBufferId(const char *file,int const id);
};
#endif //KERNEL_FACTORY_H_H
