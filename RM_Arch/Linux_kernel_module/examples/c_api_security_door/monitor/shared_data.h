#ifndef __SHARED_DATA__
#define __SHARED_DATA__

#include <stdio.h> //printf, perror
#include <stdlib.h> //exit
#include <errno.h> //errno
#include <string.h> //strerrno
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define DOOR_OPENED 0
#define DOOR_CLOSED 1

const char *name = "rtas2016";
const size_t SIZE = sizeof(int);

static inline int *config_shared_memory() {
    /* open the shared memory segment */
    int shm_fd = shm_open(name, O_RDWR | O_CREAT, 0666);
    if (shm_fd == -1) {
        perror("shared memory failed\n");
        exit(-1);
    }

    ftruncate(shm_fd, sizeof(int));

    /* now map the shared memory segment in the address space of the process */
    void *ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("Map failed\n");
        exit(-1);
    }

    return (int *) ptr;
}

static inline void free_shared_memory() {
    /* remove the shared memory segment */
    if (shm_unlink(name) == -1) {
        perror("Error removing shared memory");
        exit(-1);
    }
}

#endif /* __SHARED_DATA__ */
