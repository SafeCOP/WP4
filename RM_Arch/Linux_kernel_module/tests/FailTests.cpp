//
// Created by silverone on 6/17/15.
//

#include <errno.h>
#include <limits.h>

#include "../kmod/raven.h"
#include "FailTests.h"
#include "../lib/c/raven_api.h"

CPPUNIT_TEST_SUITE_REGISTRATION(FailTests);

void FailTests::setUp() {
    int id = raven_init();
    CPPUNIT_ASSERT(id >= 0);
}

void FailTests::tearDown() {
    int test = raven_term();
    CPPUNIT_ASSERT(test == 0);
}

static int pop(struct event_reader *pop) {
    int p = raven_pop(pop);

    CPPUNIT_ASSERT(p >= 0);

    return *(int *) pop->event_data;
}

static void push(int *data, int newdata) {
    struct event_writer event;
    *data = newdata;
    event.data = data;
    event.buffer_id = 0;
    int op = raven_push(&event);
    CPPUNIT_ASSERT(op >= 0);
}

static int peek(struct event_reader *pop) {
    int p = raven_peek(pop);

    CPPUNIT_ASSERT(p >= 0);

    return *(int *) pop->event_data;
}

static int pop_head(struct event_reader *pop) {
    int p = raven_pop_head(pop);

    CPPUNIT_ASSERT(p >= 0);

    return *(int *) pop->event_data;
}

static int peek_head(struct event_reader *pop) {
    int p = raven_peek_head(pop);

    CPPUNIT_ASSERT(p >= 0);

    return *(int *) pop->event_data;
}

void FailTests::testOverflowBufferDataBlockSize() {
    struct event_buffer eventBufer;
    eventBufer.id = 0;
    eventBufer.length = 3;
    eventBufer.index_size = USHRT_MAX;

    int op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(eventBufer.id, eventBufer.length, eventBufer.index_size);
    CPPUNIT_ASSERT(op == -1 && errno == EINVAL);
}

void FailTests::testDuplicateBufferCreation() {
    struct event_buffer eventBufer;
    eventBufer.id = 0;
    eventBufer.length = 3;
    eventBufer.index_size = 3;

    int op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(eventBufer.id, eventBufer.length, eventBufer.index_size);
    CPPUNIT_ASSERT(op == 0);

    op = raven_buffer_create(eventBufer.id, eventBufer.length, eventBufer.index_size);
    CPPUNIT_ASSERT(op == -1 && errno == EEXIST);
}

void FailTests::testZeroLengthBufferCreation() {
    struct event_buffer eventBufer;
    eventBufer.id = 0;
    eventBufer.length = 0;
    eventBufer.index_size = 3;

    int op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(eventBufer.id, eventBufer.length, eventBufer.index_size);
    CPPUNIT_ASSERT(op == -1 && errno == EINVAL);

    eventBufer.id = 0;
    eventBufer.length = 3;
    eventBufer.index_size = 0;

    op = raven_buffer_create(eventBufer.id, eventBufer.length, eventBufer.index_size);
    CPPUNIT_ASSERT(op == -1 && errno == EINVAL);
}

void FailTests::testCallOpsOnNonExistantBuffers() {
    struct event_buffer eventBufer;
    eventBufer.id = 0;
    eventBufer.length = 0;
    eventBufer.index_size = 3;

    int op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);

    //info
    struct event_buffer_info info;
    op = raven_buffer_info(eventBufer.id, &info);
    CPPUNIT_ASSERT(op == -1 && errno == ENOENT);

    //destroyBuffer
    op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT(op == -1 && errno == ENOENT);

    //push
    struct event_writer writer;
    writer.buffer_id = eventBufer.id;
    op = raven_push(&writer);
    CPPUNIT_ASSERT(op == -1 && errno == ENOENT);

    //pop
    struct event_reader reader;
    reader.id = eventBufer.id;
    op = raven_pop(&reader);
    CPPUNIT_ASSERT(op == -1 && errno == ENOENT);

    //pop_head
    op = raven_pop_head(&reader);
    CPPUNIT_ASSERT(op == -1 && errno == ENOENT);

    //peek
    op = raven_peek(&reader);
    CPPUNIT_ASSERT(op == -1 && errno == ENOENT);

    //peek_head
    op = raven_peek(&reader);
    CPPUNIT_ASSERT(op == -1 && errno == ENOENT);
}

void FailTests::testOverflowIndex() {
    struct event_buffer eventBufer;
    eventBufer.id = 0;
    eventBufer.index_size = sizeof(int);
    eventBufer.length = 3;

    int op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(0, 3, sizeof(int));
    CPPUNIT_ASSERT(op >= 0);

    int pop_data = -1;
    struct event_reader bpop;
    raven_init_event_reader(&bpop, 0, &pop_data, NULL);
    bpop.index = 9999;
    op = raven_pop(&bpop);
    CPPUNIT_ASSERT(op == -1 && errno == EINVAL);


    bpop.index = 3;
    op = raven_pop(&bpop);
    CPPUNIT_ASSERT(op == -1 && errno == EINVAL);

    op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT(op == 0);
}

void FailTests::testLastReadLargerThanSynch() {
    struct event_buffer eventBufer;
    eventBufer.id = 0;
    eventBufer.index_size = sizeof(int);
    eventBufer.length = 3;

    int op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(eventBufer.id, eventBufer.length, eventBufer.index_size);
    CPPUNIT_ASSERT(op >= 0);

    int data;
    push(&data, 1);

    int pop_data = -1;
    struct event_reader bpop;
    struct timespec time;
    time.tv_nsec = 0;
    time.tv_sec = 0;
    raven_init_event_reader(&bpop, 0, &pop_data, &time);

    CPPUNIT_ASSERT(peek(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    push(&data, 2);
    bpop.last_read.tv_nsec += 9999;
    bpop.last_read.tv_nsec += 9999;
    push(&data, 3);

    CPPUNIT_ASSERT(peek(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);


    push(&data, 4);
    push(&data, 5);
    push(&data, 6);
    bpop.last_read.tv_sec += 9999;
    bpop.last_read.tv_nsec += 9999;

    CPPUNIT_ASSERT(peek(&bpop) == 6);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 6);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);


    bpop.last_read.tv_sec += 9999;
    bpop.last_read.tv_nsec += 9999;

    push(&data, 7);
    push(&data, 8);

    CPPUNIT_ASSERT(peek_head(&bpop) == 8);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop_head(&bpop) == 8);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);


    op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT(op == 0);
}

