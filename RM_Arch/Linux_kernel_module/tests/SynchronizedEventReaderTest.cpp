#include <errno.h>

#include "SynchronizedEventReaderTest.h"
#include "../lib/c/raven_api.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SynchronizedEventReaderTest);

static void pushd(key_t buffer_id, int *data, int newdata) {
    struct event_writer event;
    *data = newdata;
    event.data = data;
    event.buffer_id = buffer_id;
    int op = raven_push(&event);
    CPPUNIT_ASSERT(op >= 0);
}

static int popd(struct event_reader *pop) {
    int p = raven_pop(pop);

    CPPUNIT_ASSERT(p >= 0);

    return *(int *) pop->event_data;
}

static int peekd(struct event_reader *pop) {
    int p = raven_peek(pop);

    CPPUNIT_ASSERT(p >= 0);

    return *(int *) pop->event_data;
}

void SynchronizedEventReaderTest::setUp() {
    TestFixture::setUp();
    int id = raven_init();
    CPPUNIT_ASSERT(id > 0);
}

void SynchronizedEventReaderTest::tearDown() {
    TestFixture::tearDown();
    int test = raven_term();
    CPPUNIT_ASSERT(test == 0);
}

/**
 * buf1 = [ ,1,2,5]
 * buf2 = [ ,3,6, ]
 *
 * pop order: 1,2,3,5, empty buf1, 6, empty buf 2
 */
void SynchronizedEventReaderTest::testNotFull1() {
    struct event_buffer eventBuffer;
    eventBuffer.id = 0;
    eventBuffer.length = 3;
    eventBuffer.index_size = sizeof(int);

    struct event_buffer eventBuffer2;
    eventBuffer2.id = 1;
    eventBuffer2.length = 3;
    eventBuffer2.index_size = sizeof(int);

    int op = raven_buffer_destroy(eventBuffer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(0, 3, sizeof(int));
    CPPUNIT_ASSERT(op >= 0);

    op = raven_buffer_destroy(eventBuffer2.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(1, 3, sizeof(int));
    CPPUNIT_ASSERT(op >= 0);

    int pop_data = -1;
    struct timespec synch;
    synch.tv_nsec = 0;
    synch.tv_sec = 0;
    struct event_reader bpop;
    raven_init_event_reader(&bpop, eventBuffer.id, &pop_data, &synch);
    struct event_reader bpop2;
    raven_init_event_reader(&bpop2, eventBuffer2.id, &pop_data, &synch);

    int push_data;
    pushd(eventBuffer.id, &push_data, 1);
    pushd(eventBuffer.id, &push_data, 2);
    pushd(eventBuffer2.id, &push_data, 3);
    pushd(eventBuffer.id, &push_data, 5);
    pushd(eventBuffer2.id, &push_data, 6);

    CPPUNIT_ASSERT(peekd(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(popd(&bpop2) == 3);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);


    CPPUNIT_ASSERT(popd(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(popd(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    CPPUNIT_ASSERT(popd(&bpop2) == 6);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 6);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);


    op = raven_buffer_destroy(eventBuffer.id);
    CPPUNIT_ASSERT(op >= 0);
    op = raven_buffer_destroy(eventBuffer2.id);
    CPPUNIT_ASSERT(op >= 0);
}

/**
 * buf1 = [ ,1,2,5]
 * buf2 = [ ,3,6, ]
 *
 * pop order: 1, 3, 6, empty buf 1, empty buf2
 */
void SynchronizedEventReaderTest::testNotFull2() {
    struct event_buffer eventBuffer;
    eventBuffer.id = 0;
    eventBuffer.length = 3;
    eventBuffer.index_size = sizeof(int);

    struct event_buffer eventBuffer2;
    eventBuffer2.id = 1;
    eventBuffer2.length = 3;
    eventBuffer2.index_size = sizeof(int);

    int op = raven_buffer_destroy(eventBuffer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(0, 3, sizeof(int));
    CPPUNIT_ASSERT(op >= 0);

    op = raven_buffer_destroy(eventBuffer2.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(1, 3, sizeof(int));
    CPPUNIT_ASSERT(op >= 0);

    int pop_data = -1;
    struct timespec synch;
    synch.tv_nsec = 0;
    synch.tv_sec = 0;
    struct event_reader bpop;
    raven_init_event_reader(&bpop, eventBuffer.id, &pop_data, &synch);
    struct event_reader bpop2;
    raven_init_event_reader(&bpop2, eventBuffer2.id, &pop_data, &synch);

    int push_data;
    pushd(eventBuffer.id, &push_data, 1);
    pushd(eventBuffer.id, &push_data, 2);
    pushd(eventBuffer2.id, &push_data, 3);
    pushd(eventBuffer.id, &push_data, 5);
    pushd(eventBuffer2.id, &push_data, 6);

    CPPUNIT_ASSERT(peekd(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peekd(&bpop2) == 3);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 3);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);

    CPPUNIT_ASSERT(peekd(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peekd(&bpop2) == 6);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 6);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);

    CPPUNIT_ASSERT(popd(&bpop) == 6);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(peekd(&bpop) == 6);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);


    CPPUNIT_ASSERT(popd(&bpop2) == 6);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);
    CPPUNIT_ASSERT(peekd(&bpop) == 6);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);


    op = raven_buffer_destroy(eventBuffer.id);
    CPPUNIT_ASSERT(op >= 0);
    op = raven_buffer_destroy(eventBuffer2.id);
    CPPUNIT_ASSERT(op >= 0);
}

/**
 * buf1 = [ ,1,2,5]
 * buf2 = [ ,3,6, ]
 *
 * pop order: 3, 5, 6, empty buf 1, empty buf2
 */
void SynchronizedEventReaderTest::testNotFull3() {
    struct event_buffer eventBuffer;
    eventBuffer.id = 0;
    eventBuffer.length = 3;
    eventBuffer.index_size = sizeof(int);

    struct event_buffer eventBuffer2;
    eventBuffer2.id = 1;
    eventBuffer2.length = 3;
    eventBuffer2.index_size = sizeof(int);

    int op = raven_buffer_destroy(eventBuffer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(0, 3, sizeof(int));
    CPPUNIT_ASSERT(op >= 0);

    op = raven_buffer_destroy(eventBuffer2.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(1, 3, sizeof(int));
    CPPUNIT_ASSERT(op >= 0);

    int pop_data = -1;
    struct timespec synch;
    synch.tv_nsec = 0;
    synch.tv_sec = 0;
    struct event_reader bpop;
    raven_init_event_reader(&bpop, eventBuffer.id, &pop_data, &synch);
    struct event_reader bpop2;
    raven_init_event_reader(&bpop2, eventBuffer2.id, &pop_data, &synch);

    int push_data;
    pushd(eventBuffer.id, &push_data, 1);
    pushd(eventBuffer.id, &push_data, 2);
    pushd(eventBuffer2.id, &push_data, 3);
    pushd(eventBuffer.id, &push_data, 5);
    pushd(eventBuffer2.id, &push_data, 6);

    CPPUNIT_ASSERT(peekd(&bpop2) == 3);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 3);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);


    CPPUNIT_ASSERT(peekd(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peekd(&bpop2) == 6);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 6);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);


    CPPUNIT_ASSERT(popd(&bpop) == 6);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(peekd(&bpop) == 6);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    CPPUNIT_ASSERT(popd(&bpop2) == 6);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);
    CPPUNIT_ASSERT(peekd(&bpop2) == 6);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);

    CPPUNIT_ASSERT(peekd(&bpop) == 6);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(popd(&bpop) == 6);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(peekd(&bpop) == 6);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(popd(&bpop2) == 6);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);


    op = raven_buffer_destroy(eventBuffer.id);
    CPPUNIT_ASSERT(op >= 0);
    op = raven_buffer_destroy(eventBuffer2.id);
    CPPUNIT_ASSERT(op >= 0);
}

/**
 * Buf1: [ ,1,2] 1,2 are popped, reader is at index 0
 * Buf1: [6,4,5] 3,4,5,6 are pushed
 * Buf2: [9,7,8] 7,8,9 are popped
 * Buf1: [6,10,11] 10,11 are popped
 *
 * No gap occurred.
 */
void SynchronizedEventReaderTest::testOverflow() {
    struct event_buffer eventBuffer;
    eventBuffer.id = 0;
    eventBuffer.length = 3;
    eventBuffer.index_size = sizeof(long long);

    struct event_buffer eventBuffer2;
    eventBuffer2.id = 1;
    eventBuffer2.length = 3;
    eventBuffer2.index_size = sizeof(long long);

    int op = raven_buffer_destroy(eventBuffer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(eventBuffer.id, eventBuffer.length, eventBuffer.index_size);
    CPPUNIT_ASSERT(op >= 0);

    op = raven_buffer_destroy(eventBuffer2.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(eventBuffer2.id, eventBuffer2.length, eventBuffer2.index_size);
    CPPUNIT_ASSERT(op >= 0);

    long long pop_data = -1;
    struct timespec synch;
    synch.tv_nsec = 0;
    synch.tv_sec = 0;
    struct event_reader bpop;
    raven_init_event_reader(&bpop, eventBuffer.id, &pop_data, &synch);
    struct event_reader bpop2;
    raven_init_event_reader(&bpop2, eventBuffer2.id, &pop_data, &synch);

    int push_data;
    pushd(eventBuffer.id, &push_data, 1);
    pushd(eventBuffer.id, &push_data, 2);

    CPPUNIT_ASSERT(peekd(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peekd(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);


    pushd(eventBuffer.id, &push_data, 3);
    pushd(eventBuffer.id, &push_data, 4);
    pushd(eventBuffer.id, &push_data, 5);
    pushd(eventBuffer.id, &push_data, 6);

    pushd(eventBuffer2.id, &push_data, 7);
    pushd(eventBuffer2.id, &push_data, 8);
    pushd(eventBuffer2.id, &push_data, 9);


    CPPUNIT_ASSERT(peekd(&bpop2) == 7);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 7);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);

    CPPUNIT_ASSERT(peekd(&bpop2) == 8);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 8);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);

    CPPUNIT_ASSERT(peekd(&bpop2) == 9);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 9);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);

    pushd(eventBuffer.id, &push_data, 10);
    pushd(eventBuffer.id, &push_data, 11);

    CPPUNIT_ASSERT(peekd(&bpop) == 10);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 10);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(peekd(&bpop) == 11);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 11);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    op = raven_buffer_destroy(eventBuffer.id);
    CPPUNIT_ASSERT(op == 0);
    op = raven_buffer_destroy(eventBuffer2.id);
    CPPUNIT_ASSERT(op == 0);
}

/**   |
 * [3,1,2] read 1
 * [3,4,5] overflow, two is lost
 * [3,4,5] three is read but with gap alert
 */
void SynchronizedEventReaderTest::testOverflow2() {
    struct event_buffer eventBufer;
    eventBufer.id = 0;
    eventBufer.length = 3;
    eventBufer.index_size = sizeof(int);

    int op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(0, 3, sizeof(int));
    CPPUNIT_ASSERT(op >= 0);

    int pop_data = -1;
    struct timespec synch;
    synch.tv_nsec = 0;
    synch.tv_sec = 0;
    struct event_reader bpop;
    raven_init_event_reader(&bpop, 0, &pop_data, &synch);

    int push_data;
    pushd(eventBufer.id, &push_data, 1);
    pushd(eventBufer.id, &push_data, 2);
    pushd(eventBufer.id, &push_data, 3);

    CPPUNIT_ASSERT(popd(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);


    pushd(eventBufer.id, &push_data, 4);
    pushd(eventBufer.id, &push_data, 5);

    CPPUNIT_ASSERT(peekd(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);


    CPPUNIT_ASSERT(peekd(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peekd(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(popd(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(peekd(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT(op >= 0);
}


/**
 *        |
 * buf1: [1,2,3] Read 1
 *          |
 *       [4,2,3] Read 2
 *                                    |
 *                          buf2:[5,6,7] Read 5,6,7
 *          |
 * buf1: [4,8,9] Read 8              |
 *                          buf2 [10,11,12] Read 11
 *            |
 * buf1: [13,14,15]
 *         |
 * buf1: [16,17,18] R 16,OF
 *                                 |
 *                          buf2: [10,11,12] empty
 *
 *                          buf2: [19,20,21]
 *                                 |
 *                          buf2: [22,23,24] OF
 */
void SynchronizedEventReaderTest::testOverflow3() {
    struct event_buffer eventBuffer;
    eventBuffer.id = 0;
    eventBuffer.length = 3;
    eventBuffer.index_size = sizeof(long long);

    struct event_buffer eventBuffer2;
    eventBuffer2.id = 1;
    eventBuffer2.length = 3;
    eventBuffer2.index_size = sizeof(long long);

    int op = raven_buffer_destroy(eventBuffer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(eventBuffer.id, eventBuffer.length, eventBuffer.index_size);
    CPPUNIT_ASSERT(op >= 0);

    op = raven_buffer_destroy(eventBuffer2.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(eventBuffer2.id, eventBuffer2.length, eventBuffer2.index_size);
    CPPUNIT_ASSERT(op >= 0);

    long long pop_data = -1;
    struct timespec synch;
    synch.tv_nsec = 0;
    synch.tv_sec = 0;
    struct event_reader bpop;
    raven_init_event_reader(&bpop, eventBuffer.id, &pop_data, &synch);
    struct event_reader bpop2;
    raven_init_event_reader(&bpop2, eventBuffer2.id, &pop_data, &synch);

    int push_data;
    pushd(eventBuffer.id, &push_data, 1);
    pushd(eventBuffer.id, &push_data, 2);
    pushd(eventBuffer.id, &push_data, 3);

    CPPUNIT_ASSERT(popd(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    pushd(eventBuffer.id, &push_data, 4);
    CPPUNIT_ASSERT(popd(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    pushd(eventBuffer2.id, &push_data, 5);
    pushd(eventBuffer2.id, &push_data, 6);
    pushd(eventBuffer2.id, &push_data, 7);

    CPPUNIT_ASSERT(popd(&bpop2) == 5);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 6);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 7);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);

    pushd(eventBuffer.id, &push_data, 8);
    pushd(eventBuffer.id, &push_data, 9);

    CPPUNIT_ASSERT(peekd(&bpop) == 8);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    pushd(eventBuffer2.id, &push_data, 10);
    pushd(eventBuffer2.id, &push_data, 11);
    pushd(eventBuffer2.id, &push_data, 12);

    CPPUNIT_ASSERT(popd(&bpop2) == 10);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 11);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);

    pushd(eventBuffer.id, &push_data, 13);
    pushd(eventBuffer.id, &push_data, 14);
    pushd(eventBuffer.id, &push_data, 15);
    pushd(eventBuffer.id, &push_data, 16);
    pushd(eventBuffer.id, &push_data, 17);
    pushd(eventBuffer.id, &push_data, 18);

    CPPUNIT_ASSERT(popd(&bpop) == 16);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);

    CPPUNIT_ASSERT(popd(&bpop2) == 16);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);

    pushd(eventBuffer2.id, &push_data, 19);
    pushd(eventBuffer2.id, &push_data, 20);
    pushd(eventBuffer2.id, &push_data, 21);
    pushd(eventBuffer2.id, &push_data, 22);
    pushd(eventBuffer2.id, &push_data, 23);
    pushd(eventBuffer2.id, &push_data, 24);

    CPPUNIT_ASSERT(popd(&bpop2) == 22);
    CPPUNIT_ASSERT(bpop2.gap == 1 && bpop2.empty == 0);

    op = raven_buffer_destroy(eventBuffer.id);
    CPPUNIT_ASSERT(op == 0);
    op = raven_buffer_destroy(eventBuffer2.id);
    CPPUNIT_ASSERT(op == 0);

}

/**
 *   [ ,1,2]     [ ,3,4]    1,3,4 are pulled
 *   [3,4,2]     [ ,3,4]    both empty
 *   [6,7,5]     [9,10,4]   5,6,9
 *   [6,7,11]    [9,10,4]   11 is pulled, buf2 empty
 *   [6,7,11]    [9,10,12]  buf1 empty, 12 is pulled
 *   [13,7,11]   [9,10,12]  13 is pulled, both are empty
 */
void SynchronizedEventReaderTest::testFull() {
    struct event_buffer eventBuffer;
    eventBuffer.id = 0;
    eventBuffer.length = 3;
    eventBuffer.index_size = sizeof(int);

    struct event_buffer eventBuffer2;
    eventBuffer2.id = 1;
    eventBuffer2.length = 3;
    eventBuffer2.index_size = sizeof(int);

    int op = raven_buffer_destroy(eventBuffer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(0, 3, sizeof(int));
    CPPUNIT_ASSERT(op >= 0);

    op = raven_buffer_destroy(eventBuffer2.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(1, 3, sizeof(int));
    CPPUNIT_ASSERT(op >= 0);

    int pop_data = -1;
    struct timespec synch;
    synch.tv_nsec = 0;
    synch.tv_sec = 0;
    struct event_reader bpop;
    raven_init_event_reader(&bpop, eventBuffer.id, &pop_data, &synch);
    struct event_reader bpop2;
    raven_init_event_reader(&bpop2, eventBuffer2.id, &pop_data, &synch);

    int push_data;
    pushd(eventBuffer.id, &push_data, 1);
    pushd(eventBuffer.id, &push_data, 2);
    pushd(eventBuffer2.id, &push_data, 3);
    pushd(eventBuffer2.id, &push_data, 4);

    CPPUNIT_ASSERT(peekd(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peekd(&bpop2) == 3);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 3);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);

    CPPUNIT_ASSERT(peekd(&bpop2) == 4);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 4);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);

    //empty now
    CPPUNIT_ASSERT(peekd(&bpop2) == 4);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);
    CPPUNIT_ASSERT(popd(&bpop2) == 4);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);
    CPPUNIT_ASSERT(popd(&bpop) == 4);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);
    CPPUNIT_ASSERT(peekd(&bpop) == 4);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);

    pushd(eventBuffer.id, &push_data, 5);
    pushd(eventBuffer.id, &push_data, 6);
    pushd(eventBuffer.id, &push_data, 7);
    pushd(eventBuffer2.id, &push_data, 9);
    pushd(eventBuffer2.id, &push_data, 10);

    CPPUNIT_ASSERT(peekd(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peekd(&bpop) == 6);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 6);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peekd(&bpop2) == 9);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 9);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);

    pushd(eventBuffer.id, &push_data, 11);
    CPPUNIT_ASSERT(peekd(&bpop) == 11);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 11);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    //empty now
    CPPUNIT_ASSERT(peekd(&bpop) == 11);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(popd(&bpop) == 11);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(popd(&bpop2) == 11);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);
    CPPUNIT_ASSERT(peekd(&bpop2) == 11);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);

    pushd(eventBuffer2.id, &push_data, 12);
    pushd(eventBuffer.id, &push_data, 13);

    CPPUNIT_ASSERT(peekd(&bpop2) == 12);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop2) == 12);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 0);
    CPPUNIT_ASSERT(peekd(&bpop) == 13);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(popd(&bpop) == 13);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    //empty now
    CPPUNIT_ASSERT(popd(&bpop2) == 13);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);
    CPPUNIT_ASSERT(peekd(&bpop2) == 13);
    CPPUNIT_ASSERT(bpop2.gap == 0 && bpop2.empty == 1);
    CPPUNIT_ASSERT(popd(&bpop) == 13);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    op = raven_buffer_destroy(eventBuffer.id);
    CPPUNIT_ASSERT(op >= 0);
    op = raven_buffer_destroy(eventBuffer2.id);
    CPPUNIT_ASSERT(op >= 0);
}
