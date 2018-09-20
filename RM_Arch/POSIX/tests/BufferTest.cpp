#include "BufferTest.h"
#include "../Buffer.h"

CPPUNIT_TEST_SUITE_REGISTRATION(BufferTest);

void BufferTest::setUp() {
    TestFixture::setUp();
}

void BufferTest::tearDown() {
    TestFixture::tearDown();
}

void BufferTest::testIsEmpty() {
    struct timespec gettime_now;
    clock_gettime(CLOCK_REALTIME, &gettime_now);
    Event<std::string> buf[5];
    for (size_t i = 0; i < 5; i++) {
        buf[i].getTime().tv_nsec = 0;
        buf[i].getTime().tv_sec = 0;
    }

    Buffer<std::string> buffer(buf, 4);
    CPPUNIT_ASSERT(buffer.empty());
    Event<std::string> event("Hello world!", gettime_now);
    buffer.push(event);
    CPPUNIT_ASSERT(!buffer.empty());
}

void BufferTest::testGetLength() {
    Event<int> buf[10];
    Buffer<int> buffer(buf, 10);
    for (size_t i = 0; i < 10; i++) {
        buf[i].getTime().tv_nsec = 0;
        buf[i].getTime().tv_sec = 0;
    }
    unsigned long length = buffer.getLength();
    CPPUNIT_ASSERT(length == 10);
}

void BufferTest::testBuffer() {
    Event<int> buf[3];
    for (int i = 0; i < 3; i++) {
        buf[i].getTime().tv_nsec = 0;
        buf[i].getTime().tv_sec = 0;
    }

    Buffer<int> buffer(buf, 3);

    Event<int> head;
    head.getTime().tv_nsec = 0;
    head.getTime().tv_sec = 0;
    CPPUNIT_ASSERT(head.isNull());

    struct timespec lo;
    buffer.readLastOverwritten(lo);

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> four(4);
    Event<int> five(5);

    buffer.push(one);
    buffer.readIndex(head, buffer.getHead());
    CPPUNIT_ASSERT(head.getData() == one.getData());

    buffer.readLastOverwritten(lo);
    CPPUNIT_ASSERT(lo.tv_nsec == 0 && lo.tv_sec == 0);

    buffer.push(two);
    buffer.readIndex(head, buffer.getHead());
    CPPUNIT_ASSERT(head.getData() == two.getData());
    buffer.readLastOverwritten(lo);
    CPPUNIT_ASSERT(lo.tv_nsec == 0 && lo.tv_sec == 0);

    buffer.push(three);
    buffer.readIndex(head, buffer.getHead());
    CPPUNIT_ASSERT(head.getData() == three.getData());
    buffer.readLastOverwritten(lo);
    CPPUNIT_ASSERT(lo.tv_nsec == 0 && lo.tv_sec == 0);


    buffer.push(four);
    buffer.readLastOverwritten(lo);
    buffer.readIndex(head, buffer.getHead());
    CPPUNIT_ASSERT(head.getData() == four.getData());
    CPPUNIT_ASSERT(lo == one.getTime());


    buffer.push(five);
    buffer.readLastOverwritten(lo);
    buffer.readIndex(head, buffer.getHead());
    CPPUNIT_ASSERT(head.getData() == five.getData());
    CPPUNIT_ASSERT(lo == two.getTime());

    buffer.push(6);
    buffer.readIndex(head, buffer.getHead());
    CPPUNIT_ASSERT(head.getData() == 6);
}
