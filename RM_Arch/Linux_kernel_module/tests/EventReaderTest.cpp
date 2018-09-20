#include <errno.h>
#include "EventReaderTest.h"
#include "../lib/c/raven_api.h"

CPPUNIT_TEST_SUITE_REGISTRATION(EventReaderTest);

static void push(int *data, int newdata) {
    struct event_writer event;
    *data = newdata;
    event.data = data;
    event.buffer_id = 0;
    int op = raven_push(&event);
    CPPUNIT_ASSERT(op >= 0);
}

static int pop(struct event_reader *pop) {
    int p = raven_pop(pop);

    CPPUNIT_ASSERT(p >= 0);

    return *(int *) pop->event_data;
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

void EventReaderTest::setUp() {
    int id = raven_init();
    CPPUNIT_ASSERT(id > 0);
}


void EventReaderTest::tearDown() {
    int test = raven_term();
    CPPUNIT_ASSERT(test == 0);
}


void EventReaderTest::testEmpty() {
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

    pop(&bpop);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    peek(&bpop);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    int push_data;
    push(&push_data, 1);
    CPPUNIT_ASSERT(peek(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.empty == 0 && bpop.gap == 0);

    op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT(op >= 0);
}

void EventReaderTest::testHalfFull() {
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

    int push_data;
    push(&push_data, 1);

    CPPUNIT_ASSERT(peek(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    //buffer is not empty, but the reader has read all the elements
    CPPUNIT_ASSERT(peek(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(pop(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(peek(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    //reader is no longer empty
    push(&push_data, 2);
    CPPUNIT_ASSERT(peek(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT(op >= 0);
}

/*
 * Buffer as elements 1,2,3 written to it, number 1 has been read
 * and will be overwritten with 4
 *    |
 * [3,1,2] last read is 1
 *
 * Writer writes 4
 *
 *    |
 * [3,4,2] Last read is 1, last overwritten is 1
 *
 * No overflow occurred thus no exception should be thrown.
 */
void EventReaderTest::testFullOne() {
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

    pop(&bpop);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    int push_data;
    push(&push_data, 1);
    push(&push_data, 2);
    push(&push_data, 3);

    CPPUNIT_ASSERT(peek(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    push(&push_data, 4);

    CPPUNIT_ASSERT(peek(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);


    CPPUNIT_ASSERT(peek(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);


    CPPUNIT_ASSERT(peek(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(pop(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(peek(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(pop(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(peek(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT(op == 0);
}

/*
 * Buffer as elements 1,2,3 written to it, number 1 hasn't been read
 * and will be overwritten with 4, since the buffer hasn't read anything, no exception should be thrown.
 *    |
 * [3,1,2] last read is 1
 *
 * Writer writes 4
 *
 *    |
 * [3,4,2] Last read is 1, last overwritten is 1
 *
 * No overflow occurred thus no exception should be thrown.
 */
void EventReaderTest::testFullTwo() {
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

    int push_data;
    push(&push_data, 1);
    push(&push_data, 2);
    push(&push_data, 3);
    push(&push_data, 4);

    CPPUNIT_ASSERT(peek(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(pop(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(peek(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT(op >= 0);
}

/*
 * Buffer has elements 1,2,3 written to it, reader reads element nr1.
 * We then write 4 and 5, resulting in overflow. Exception should be thrown.
 *    |
 * [3,1,2] last read=1
 * [3,4,5] last read=1, last overwritten=2
 */
void EventReaderTest::testFullThree() {
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

    int push_data;
    push(&push_data, 1);
    CPPUNIT_ASSERT(pop(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    push(&push_data, 2);
    push(&push_data, 3);
    push(&push_data, 4);
    push(&push_data, 5);


    CPPUNIT_ASSERT(peek(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(pop(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(peek(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT(op >= 0);
}

/*
 * Buffer has elements 1,2,3 written to it, reader reads up to element nr3.
 * We then write 3-12, resulting in overflow. Exception should be thrown,
 * elements 10, 11, 12 should pop out next.
 *  |
 * [3,1,2] last read=3
 * ...
 * [10,11,512 last read=2(where 10 is), last overwritten=9
 */
void EventReaderTest::testFullFour() {
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

    int push_data;
    push(&push_data, 1);
    CPPUNIT_ASSERT(peek(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(peek(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    push(&push_data, 2);
    CPPUNIT_ASSERT(peek(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(peek(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    push(&push_data, 3);
    push(&push_data, 4);
    push(&push_data, 5);

    CPPUNIT_ASSERT(peek(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(pop(&bpop) == 5);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT(op >= 0);
}

/*
 * Buffer has elements 1,2,3 written to it, reader reads up to element nr3.
 * We then write 3-12, resulting in overflow. Exception should be thrown,
 * elements 10, 11, 12 should pop out next.
 *  |
 * [3,1,2] last read=3
 * ...
 * [10,11,512 last read=2(where 10 is), last overwritten=9
 */
void EventReaderTest::testFullFive() {
    struct event_buffer eventBufer;
    eventBufer.id = 0;
    eventBufer.index_size = sizeof(int);
    eventBufer.length = 3;

    int op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT((op == -1 && errno == ENOENT) || op == 0);
    op = raven_buffer_create(eventBufer.id, eventBufer.length, eventBufer.index_size);
    CPPUNIT_ASSERT(op >= 0);

    int pop_data = -1;
    struct event_reader bpop;
    raven_init_event_reader(&bpop, 0, &pop_data, NULL);

    int push_data;
    push(&push_data, 1);
    push(&push_data, 2);
    push(&push_data, 3);
    CPPUNIT_ASSERT(peek(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 2);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(pop(&bpop) == 3);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    push(&push_data, 4);
    push(&push_data, 5);
    push(&push_data, 6);
    push(&push_data, 7);
    push(&push_data, 8);
    push(&push_data, 9);
    push(&push_data, 10);
    push(&push_data, 11);
    push(&push_data, 12);


    CPPUNIT_ASSERT(peek(&bpop) == 10);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 10);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 11);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 11);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 12);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop(&bpop) == 12);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek(&bpop) == 12);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(pop(&bpop) == 12);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT(op >= 0);
}


void EventReaderTest::testPopHead() {
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

    int push_data;
    push(&push_data, 1);
    CPPUNIT_ASSERT(peek_head(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop_head(&bpop) == 1);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    push(&push_data, 2);
    push(&push_data, 3);
    push(&push_data, 4);
    CPPUNIT_ASSERT(peek_head(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop_head(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 0);

    //empty
    CPPUNIT_ASSERT(peek_head(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(pop_head(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(peek_head(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(pop_head(&bpop) == 4);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);


    push(&push_data, 5);
    push(&push_data, 6);
    push(&push_data, 7);
    push(&push_data, 8);

    CPPUNIT_ASSERT(peek_head(&bpop) == 8);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop_head(&bpop) == 8);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);
    CPPUNIT_ASSERT(peek_head(&bpop) == 8);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(pop_head(&bpop) == 8);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    push(&push_data, 9);
    push(&push_data, 10);
    push(&push_data, 11);
    push(&push_data, 12);

    CPPUNIT_ASSERT(peek_head(&bpop) == 12);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);
    CPPUNIT_ASSERT(pop_head(&bpop) == 12);
    CPPUNIT_ASSERT(bpop.gap == 1 && bpop.empty == 0);

    CPPUNIT_ASSERT(peek_head(&bpop) == 12);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);
    CPPUNIT_ASSERT(pop_head(&bpop) == 12);
    CPPUNIT_ASSERT(bpop.gap == 0 && bpop.empty == 1);

    op = raven_buffer_destroy(eventBufer.id);
    CPPUNIT_ASSERT(op >= 0);
}

