#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "RavenOperationsTest.h"
#include "../kmod/raven.h"
#include "../lib/c/raven_api.h"

CPPUNIT_TEST_SUITE_REGISTRATION(RavenOperationsTest);

void RavenOperationsTest::setUp() {
    TestFixture::setUp();

    int id = raven_init();
    CPPUNIT_ASSERT(id > 0);
}

void RavenOperationsTest::tearDown() {
    TestFixture::tearDown();
}

void RavenOperationsTest::testGetInfo() {
    struct event_buffer buf;
    buf.id = 0;
    buf.index_size = 4;
    buf.length = 10;

    int test = raven_buffer_destroy(buf.id);
    CPPUNIT_ASSERT((test == -1 && errno == ENOENT) || test == 0);
    test = raven_buffer_create(0, 10, 4);
    CPPUNIT_ASSERT(test == 0);

    int data = 5;
    struct event_writer event;
    event.data = &data;
    event.buffer_id = 0;
    test = raven_push(&event);
    CPPUNIT_ASSERT(test >= 0);

    struct event_buffer_info buf_test;
    test = raven_buffer_info(buf.id, &buf_test);
    CPPUNIT_ASSERT(test == 0);

    CPPUNIT_ASSERT(buf_test.length == buf.length);
    CPPUNIT_ASSERT(buf_test.index_size == buf.index_size);
    CPPUNIT_ASSERT(buf_test.last_overwritten.tv_nsec == 0 && buf_test.last_overwritten.tv_sec == 0);
    CPPUNIT_ASSERT(buf_test.id == buf.id);
    CPPUNIT_ASSERT(buf_test.owner == getpid());

    int op = raven_buffer_destroy(buf.id);
    CPPUNIT_ASSERT(op == 0);
}

void RavenOperationsTest::testList() {
    struct event_buffer buf[3];
    buf[0].id = 0;
    buf[0].index_size = 4;
    buf[0].length = 10;

    int test = raven_buffer_destroy(buf[0].id);
    CPPUNIT_ASSERT((test == -1 && errno == ENOENT) || test == 0);
    test = raven_buffer_create(0, 10, 4);
    CPPUNIT_ASSERT(test == 0);

    int data = 5;
    struct event_writer event;
    event.data = &data;
    event.buffer_id = 0;
    test = raven_push(&event);
    CPPUNIT_ASSERT(test >= 0);

    /////////////////////

    buf[1].id = 1;
    buf[1].index_size = 2;
    buf[1].length = 16;

    test = raven_buffer_destroy(buf[1].id);
    CPPUNIT_ASSERT((test == -1 && errno == ENOENT) || test == 0);
    test = raven_buffer_create(1, 16, 2);
    CPPUNIT_ASSERT(test == 0);

    short data2 = 16;
    struct event_writer event2;
    event2.data = &data2;
    event2.buffer_id = 1;
    test = raven_push(&event2);
    CPPUNIT_ASSERT(test >= 0);

    /////////////////////
    buf[2].id = 2;
    buf[2].index_size = 1;
    buf[2].length = 16;

    test = raven_buffer_destroy(buf[2].id);
    CPPUNIT_ASSERT((test == -1 && errno == ENOENT) || test == 0);
    test = raven_buffer_create(2, 16, 1);
    CPPUNIT_ASSERT(test == 0);

    unsigned char data3 = 16;
    struct event_writer event3;
    event3.data = &data3;
    event3.buffer_id = 2;
    test = raven_push(&event3);
    CPPUNIT_ASSERT(test >= 0);


    struct event_buffer_info event_buffers[3];
    memset(&event_buffers, 0, sizeof(struct event_buffer_info) * 3);

    struct event_buffer_list list;

    test = raven_list_buffers(&list, event_buffers, 3);
    CPPUNIT_ASSERT(test >= 0);
    CPPUNIT_ASSERT(list.number_of_buffers == 3);
    CPPUNIT_ASSERT(list.event_buffers_length == 3);

    CPPUNIT_ASSERT(buf[0].length == event_buffers[0].length);
    CPPUNIT_ASSERT(buf[0].index_size == event_buffers[0].index_size);
    CPPUNIT_ASSERT(event_buffers[0].last_overwritten.tv_nsec == 0 && event_buffers[0].last_overwritten.tv_sec == 0);
    CPPUNIT_ASSERT(buf[0].id == event_buffers[0].id);
    CPPUNIT_ASSERT(event_buffers[0].owner == getpid());

    CPPUNIT_ASSERT(buf[2].id == event_buffers[1].id);
    CPPUNIT_ASSERT(buf[2].length == event_buffers[1].length);
    CPPUNIT_ASSERT(buf[2].index_size == event_buffers[1].index_size);
    CPPUNIT_ASSERT(event_buffers[1].last_overwritten.tv_nsec == 0 && event_buffers[1].last_overwritten.tv_sec == 0);
    CPPUNIT_ASSERT(event_buffers[1].owner == getpid());

    CPPUNIT_ASSERT(buf[1].length == event_buffers[2].length);
    CPPUNIT_ASSERT(buf[1].index_size == event_buffers[2].index_size);
    CPPUNIT_ASSERT(event_buffers[2].last_overwritten.tv_nsec == 0 && event_buffers[2].last_overwritten.tv_sec == 0);
    CPPUNIT_ASSERT(buf[1].id == event_buffers[2].id);
    CPPUNIT_ASSERT(event_buffers[2].owner == getpid());

    for (size_t i = 0; i < 3; i++) {
        int op = raven_buffer_destroy(buf[i].id);
        CPPUNIT_ASSERT(op == 0);
    }
}

void RavenOperationsTest::testListZeroLengthArray() {
    struct event_buffer buf[3];
    buf[0].id = 0;
    buf[0].index_size = 4;
    buf[0].length = 10;

    int test = raven_buffer_destroy(buf[0].id);
    CPPUNIT_ASSERT((test == -1 && errno == ENOENT) || test == 0);
    test = raven_buffer_create(0, 10, 4);
    CPPUNIT_ASSERT(test == 0);

    int data = 5;
    struct event_writer event;
    event.data = &data;
    event.buffer_id = 0;
    test = raven_push(&event);
    CPPUNIT_ASSERT(test >= 0);

    /////////////////////

    buf[1].id = 1;
    buf[1].index_size = 2;
    buf[1].length = 16;

    test = raven_buffer_destroy(buf[1].id);
    CPPUNIT_ASSERT((test == -1 && errno == ENOENT) || test == 0);
    test = raven_buffer_create(1, 16, 2);
    CPPUNIT_ASSERT(test == 0);

    short data2 = 16;
    struct event_writer event2;
    event2.data = &data2;
    event2.buffer_id = 1;
    test = raven_push(&event2);
    CPPUNIT_ASSERT(test >= 0);

    /////////////////////
    buf[2].id = 2;
    buf[2].index_size = 1;
    buf[2].length = 16;

    test = raven_buffer_destroy(buf[2].id);
    CPPUNIT_ASSERT((test == -1 && errno == ENOENT) || test == 0);
    test = raven_buffer_create(2, 16, 1);
    CPPUNIT_ASSERT(test == 0);

    unsigned char data3 = 16;
    struct event_writer event3;
    event3.data = &data3;
    event3.buffer_id = 2;
    test = raven_push(&event3);
    CPPUNIT_ASSERT(test >= 0);


    struct event_buffer_list list;

    test = raven_list_buffers(&list, NULL, 0);
    CPPUNIT_ASSERT(test >= 0);
    CPPUNIT_ASSERT(list.number_of_buffers == 3);
    CPPUNIT_ASSERT(list.event_buffers_length == 0);

    for (size_t i = 0; i < 3; i++) {
        int op = raven_buffer_destroy(buf[i].id);
        CPPUNIT_ASSERT(op == 0);
    }
}

void RavenOperationsTest::testListOverflow() {

    struct event_buffer buf[3];
    buf[0].id = 0;
    buf[0].index_size = sizeof(int);
    buf[0].length = 10;

    int test = raven_buffer_destroy(buf[0].id);
    CPPUNIT_ASSERT((test == -1 && errno == ENOENT) || test == 0);
    test = raven_buffer_create(buf[0].id, buf[0].length, buf[0].index_size);
    CPPUNIT_ASSERT(test == 0);

    int data = 5;
    struct event_writer event;
    event.data = &data;
    event.buffer_id = 0;
    test = raven_push(&event);
    CPPUNIT_ASSERT(test >= 0);

    /////////////////////

    buf[1].id = 1;
    buf[1].index_size = sizeof(short);
    buf[1].length = 16;

    test = raven_buffer_destroy(buf[1].id);
    CPPUNIT_ASSERT((test == -1 && errno == ENOENT) || test == 0);
    test = raven_buffer_create(buf[1].id, buf[1].length, buf[1].index_size);
    CPPUNIT_ASSERT(test == 0);

    short data2 = 16;
    struct event_writer event2;
    event2.data = &data2;
    event2.buffer_id = 1;
    test = raven_push(&event2);
    CPPUNIT_ASSERT(test >= 0);

    /////////////////////
    buf[2].id = 2;
    buf[2].index_size = sizeof(unsigned char);
    buf[2].length = 16;

    test = raven_buffer_destroy(buf[2].id);
    CPPUNIT_ASSERT((test == -1 && errno == ENOENT) || test == 0);
    test = raven_buffer_create(buf[2].id, buf[2].length, buf[2].index_size);
    CPPUNIT_ASSERT(test == 0);

    unsigned char data3 = 16;
    struct event_writer event3;
    event3.data = &data3;
    event3.buffer_id = 2;
    test = raven_push(&event3);
    CPPUNIT_ASSERT(test >= 0);


    struct event_buffer_info event_buffers[2];
    memset(&event_buffers, 0, sizeof(struct event_buffer_info) * 2);

    struct event_buffer_list list;

    test = raven_list_buffers(&list, event_buffers, 2);
    CPPUNIT_ASSERT(test >= 0);
    CPPUNIT_ASSERT(list.number_of_buffers == 3);
    CPPUNIT_ASSERT(list.event_buffers_length == 2);

    CPPUNIT_ASSERT(buf[0].length == event_buffers[0].length);
    CPPUNIT_ASSERT(buf[0].index_size == event_buffers[0].index_size);
    CPPUNIT_ASSERT(event_buffers[0].last_overwritten.tv_nsec == 0 && event_buffers[0].last_overwritten.tv_sec == 0);
    CPPUNIT_ASSERT(buf[0].id == event_buffers[0].id);
    CPPUNIT_ASSERT(event_buffers[0].owner == getpid());

    CPPUNIT_ASSERT(buf[2].id == event_buffers[1].id);
    CPPUNIT_ASSERT(buf[2].length == event_buffers[1].length);
    CPPUNIT_ASSERT(buf[2].index_size == event_buffers[1].index_size);
    CPPUNIT_ASSERT(event_buffers[1].last_overwritten.tv_nsec == 0 && event_buffers[1].last_overwritten.tv_sec == 0);
    CPPUNIT_ASSERT(event_buffers[1].owner == getpid());

    for (size_t i = 0; i < 3; i++) {
        int op = raven_buffer_destroy(buf[i].id);
        CPPUNIT_ASSERT(op == 0);
    }
}

void RavenOperationsTest::testCreateAndDestroy() {
    int test = raven_buffer_destroy(0);
    CPPUNIT_ASSERT((test == -1 && errno == ENOENT) || test == 0);
    test = raven_buffer_create(0, 10, 4);
    CPPUNIT_ASSERT(test == 0);
}
