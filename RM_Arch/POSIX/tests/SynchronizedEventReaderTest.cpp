#include <cppunit/TestAssert.h>
#include "SynchronizedEventReaderTest.h"
#include "../EventBuffer.h"

CPPUNIT_TEST_SUITE_REGISTRATION(SynchronizedEventReaderTest);

void SynchronizedEventReaderTest::setUp() {
    TestFixture::setUp();
}

void SynchronizedEventReaderTest::tearDown() {
    TestFixture::tearDown();
}

/**
 * buf1 = [ ,1,2,5]
 * buf2 = [ ,3,6, ]
 *
 * pop order: 1,2,3,5, empty buf1, 6, empty buf 2
 */
void SynchronizedEventReaderTest::testNotFull1() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    EventBuffer<int, 4> eventBuffer1;
    EventBuffer<int, 4> eventBuffer2;

    Event<int> one(1);
    Event<int> two(2);
    Event<int> five(5);
    Event<int> three(3);
    Event<int> six(6);

    EventWriter<int> eventWriter1;
    eventBuffer1.configWriter(eventWriter1);
    EventWriter<int> eventWriter2;
    eventBuffer2.configWriter(eventWriter2);

    eventWriter1.push(one);
    eventWriter1.push(two);
    eventWriter1.push(five);

    eventWriter2.push(three);
    eventWriter2.push(six);

    SynchronizedEventReader<int> eventReader1;
    eventBuffer1.configSynchronizedEventReader(&time, eventReader1);
    SynchronizedEventReader<int> eventReader2;
    eventBuffer2.configSynchronizedEventReader(&time, eventReader2);

    Event<int> event;
    bool updated;
    bool gap;

    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);

    //empty
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(!updated && !gap);

    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(event == six);

    //empty
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(!updated && !gap);
}


/**
 * buf1 = [ ,1,2,5]
 * buf2 = [ ,3,6, ]
 *
 * pop order: 1, 3, 6, empty buf 1, empty buf2
 */
void SynchronizedEventReaderTest::testNotFull2() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);

    EventBuffer<int, 4> eventBuffer1;
    EventBuffer<int, 4> eventBuffer2;

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> five(5);
    Event<int> six(6);

    EventWriter<int> eventWriter1;
    eventBuffer1.configWriter(eventWriter1);
    EventWriter<int> eventWriter2;
    eventBuffer2.configWriter(eventWriter2);

    eventWriter1.push(one);
    eventWriter1.push(two);
    eventWriter1.push(five);

    eventWriter2.push(three);
    eventWriter2.push(six);

    SynchronizedEventReader<int> eventReader1;
    eventBuffer1.configSynchronizedEventReader(&time, eventReader1);
    SynchronizedEventReader<int> eventReader2;
    eventBuffer2.configSynchronizedEventReader(&time, eventReader2);

    Event<int> event;
    bool updated;
    bool gap;

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(event == three);
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(event == three);

    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(updated && !gap);

    //empty
    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(!updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(!updated && !gap);
    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(!updated && !gap);
}

/**
 * buf1 = [ ,1,2,5]
 * buf2 = [ ,3,6, ]
 *
 * pop order: 3, 5, 6, empty buf 1, empty buf2
 */
void SynchronizedEventReaderTest::testNotFull3() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);

    EventBuffer<int, 4> eventBuffer1;
    EventBuffer<int, 4> eventBuffer2;

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> five(5);
    Event<int> six(6);

    EventWriter<int> eventWriter1;
    eventBuffer1.configWriter(eventWriter1);
    EventWriter<int> eventWriter2;
    eventBuffer2.configWriter(eventWriter2);

    eventWriter1.push(one);
    eventWriter1.push(two);
    eventWriter1.push(five);

    eventWriter2.push(three);
    eventWriter2.push(six);

    SynchronizedEventReader<int> eventReader1;
    eventBuffer1.configSynchronizedEventReader(&time, eventReader1);
    SynchronizedEventReader<int> eventReader2;
    eventBuffer2.configSynchronizedEventReader(&time, eventReader2);

    Event<int> event;
    bool updated;
    bool gap;

    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(updated && !gap);

    //empty
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(!updated && !gap);
}

/**
 *          |
 * buf1: [3,1,2]
 * element 1 will be replaced by element 4 with no overflow
 *
 *          |
 * buf2: [3,1,2]
 * element 1 will be replaced  by element 4 with no overflow
 * element 2 will be replaced by element  5 with local overflow
 */
void SynchronizedEventReaderTest::testOverflow() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    EventBuffer<int, 3> eventBuffer1;
    EventBuffer<int, 3> eventBuffer2;

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> four(4);
    Event<int> five(5);
    Event<int> six(6);

    EventWriter<int> eventWriter1;
    eventBuffer1.configWriter(eventWriter1);
    EventWriter<int> eventWriter2;
    eventBuffer2.configWriter(eventWriter2);

    eventWriter1.push(one);
    eventWriter1.push(two);
    eventWriter1.push(three);
    eventWriter2.push(one);
    eventWriter2.push(two);
    eventWriter2.push(three);

    SynchronizedEventReader<int> eventReader1;
    eventBuffer1.configSynchronizedEventReader(&time, eventReader1);
    SynchronizedEventReader<int> eventReader2;
    eventBuffer2.configSynchronizedEventReader(&time, eventReader2);

    Event<int> event;
    bool updated;
    bool gap;

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && !gap);

    eventWriter1.push(four);
    eventWriter2.push(four);
    eventWriter2.push(five);
    eventWriter2.push(six);

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(updated && !gap);
}


/**   |
 * [3,1,2] read 1
 * [3,4,5] overflow, two is lost
 * [3,4,5] three is read but with gap alert
 */
void SynchronizedEventReaderTest::testOverflow2() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    EventBuffer<int, 3> eventBuffer1;

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> four(4);
    Event<int> five(5);

    EventWriter<int> eventWriter;
    eventBuffer1.configWriter(eventWriter);
    SynchronizedEventReader<int> eventReader;
    eventBuffer1.configSynchronizedEventReader(&time, eventReader);

    eventWriter.push(one);
    eventWriter.push(two);
    eventWriter.push(three);

    Event<int> event;
    bool gap;
    bool updated = eventReader.pop(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(event == one);

    eventWriter.push(four);
    eventWriter.push(five);

    updated = eventReader.pop(event, gap);
    CPPUNIT_ASSERT(updated && gap);
    CPPUNIT_ASSERT(event == three);

    updated = eventReader.pop(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(event == four);

    updated = eventReader.pop(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(event == five);
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
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    EventBuffer<int, 3> eventBuffer1;
    EventBuffer<int, 3> eventBuffer2;

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> four(4);
    Event<int> five(5);
    Event<int> six(6);
    Event<int> seven(7);
    Event<int> nine(9);
    Event<int> ten(10);
    Event<int> eleven(11);
    Event<int> twelve(12);
    Event<int> thirteen(13);

    EventWriter<int> eventWriter1;
    eventBuffer1.configWriter(eventWriter1);
    EventWriter<int> eventWriter2;
    eventBuffer2.configWriter(eventWriter2);

    eventWriter1.push(one);
    eventWriter1.push(two);
    eventWriter2.push(three);
    eventWriter2.push(four);

    SynchronizedEventReader<int> eventReader1;
    eventBuffer1.configSynchronizedEventReader(&time, eventReader1);
    SynchronizedEventReader<int> eventReader2;
    eventBuffer2.configSynchronizedEventReader(&time, eventReader2);

    Event<int> event;
    bool updated;
    bool gap;

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);

    //empty
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(!updated && !gap);

    eventWriter1.push(five);
    eventWriter1.push(six);
    eventWriter1.push(seven);
    eventWriter2.push(nine);
    eventWriter2.push(ten);

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == six);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == nine);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == nine);
    CPPUNIT_ASSERT(updated && !gap);

    eventWriter1.push(eleven);

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == eleven);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == eleven);
    CPPUNIT_ASSERT(updated && !gap);


    //empty
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == eleven);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == eleven);
    CPPUNIT_ASSERT(!updated && !gap);

    eventWriter2.push(twelve);
    eventWriter1.push(thirteen);

    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == twelve);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == twelve);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == thirteen);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == thirteen);
    CPPUNIT_ASSERT(updated && !gap);

    //empty
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == thirteen);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == thirteen);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == thirteen);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == thirteen);
    CPPUNIT_ASSERT(!updated && !gap);
}

/**
 * Tests buffer empty method.
 */
void SynchronizedEventReaderTest::testEmpty() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    EventBuffer<int, 3> eventBuffer1;

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> four(4);
    Event<int> five(5);

    EventWriter<int> eventWriter;
    eventBuffer1.configWriter(eventWriter);
    SynchronizedEventReader<int> eventReader;
    eventBuffer1.configSynchronizedEventReader(&time, eventReader);

    Event<int> event;
    bool updated;
    bool gap;

    CPPUNIT_ASSERT(eventReader.empty());
    eventWriter.push(one);
    CPPUNIT_ASSERT(!eventReader.empty());
    updated = eventReader.peek(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(!eventReader.empty());
    updated = eventReader.pop(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(eventReader.empty());

    eventWriter.push(two);
    eventWriter.push(three);
    eventWriter.push(four);
    CPPUNIT_ASSERT(!eventReader.empty());
    updated = eventReader.peek(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(!eventReader.empty());
    updated = eventReader.pop(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(!eventReader.empty());

    updated = eventReader.peek(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(!eventReader.empty());
    updated = eventReader.pop(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(!eventReader.empty());

    updated = eventReader.peek(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(!eventReader.empty());
    updated = eventReader.pop(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(eventReader.empty());


    //empty
    updated = eventReader.peek(event, gap);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader.pop(event, gap);
    CPPUNIT_ASSERT(!updated && !gap);

    //time updated thus buffer is empty
    eventWriter.push(five);
    clock_gettime(CLOCK_MONOTONIC, &time);
    CPPUNIT_ASSERT(eventReader.empty());

    //empty
    updated = eventReader.pop(event, gap);
    CPPUNIT_ASSERT(!updated && !gap);

    //empty
    updated = eventReader.peek(event, gap);
    CPPUNIT_ASSERT(!updated && !gap);
}

/**
 * Tests popHead / peekHead.
 */
void SynchronizedEventReaderTest::testHead() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    EventBuffer<int, 3> eventBuffer1;

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> four(4);
    Event<int> five(5);
    Event<int> six(6);
    Event<int> seven(7);
    Event<int> eight(8);

    EventWriter<int> eventWriter;
    eventBuffer1.configWriter(eventWriter);
    SynchronizedEventReader<int> eventReader;
    eventBuffer1.configSynchronizedEventReader(&time, eventReader);

    Event<int> event;
    bool updated;
    bool gap;

    updated = eventReader.popHead(event, gap);
    CPPUNIT_ASSERT(!updated && !gap);


    updated = eventReader.peekHead(event, gap);
    CPPUNIT_ASSERT(!updated && !gap);


    eventWriter.push(one);
    updated = eventReader.peekHead(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader.popHead(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);

    //empty
    updated = eventReader.popHead(event, gap);
    CPPUNIT_ASSERT(!updated && !gap);
    CPPUNIT_ASSERT(event == one);
    updated = eventReader.peekHead(event, gap);
    CPPUNIT_ASSERT(!updated && !gap);

    eventWriter.push(two);
    eventWriter.push(three);
    eventWriter.push(four);

    updated = eventReader.peekHead(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader.popHead(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);

    eventWriter.push(five);
    eventWriter.push(six);
    eventWriter.push(seven);
    eventWriter.push(eight);

    //overflow
    updated = eventReader.peekHead(event, gap);
    CPPUNIT_ASSERT(event == eight);
    CPPUNIT_ASSERT(updated && gap);
    updated = eventReader.popHead(event, gap);
    CPPUNIT_ASSERT(event == eight);
    CPPUNIT_ASSERT(updated && gap);
}

/**
 * Buf1: [ ,1,2] 1,2 are popped, reader is at index 0
 * Buf1: [6,4,5] 3,4,5,6 are pushed
 * Buf2: [9,7,8] 7,8,9 are popped
 * Buf1: [6,10,11] 10,11 are popped
 *
 * No gap occurred.
 */
void SynchronizedEventReaderTest::testOverflow3() {
    struct timespec synch = {0};

    EventBuffer<int, 3> eventBuffer1;
    EventWriter<int> eventWriter1;
    eventBuffer1.configWriter(eventWriter1);
    SynchronizedEventReader<int> eventReader1;
    eventBuffer1.configSynchronizedEventReader(&synch, eventReader1);

    EventBuffer<int, 3> eventBuffer2;
    EventWriter<int> eventWriter2;
    eventBuffer2.configWriter(eventWriter2);
    SynchronizedEventReader<int> eventReader2;
    eventBuffer2.configSynchronizedEventReader(&synch, eventReader2);

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> four(4);
    Event<int> five(5);
    Event<int> six(6);
    Event<int> seven(7);
    Event<int> eight(8);
    Event<int> nine(9);
    Event<int> ten(10);
    Event<int> eleven(11);

    Event<int> event;
    bool updated;
    bool gap;

    eventWriter1.push(one);
    eventWriter1.push(two);


    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && !gap);

    eventWriter1.push(three);
    eventWriter1.push(four);
    eventWriter1.push(five);
    eventWriter1.push(six);

    eventWriter2.push(seven);
    eventWriter2.push(eight);
    eventWriter2.push(nine);

    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == seven);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == seven);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == eight);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == eight);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader2.peek(event, gap);
    CPPUNIT_ASSERT(event == nine);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader2.pop(event, gap);
    CPPUNIT_ASSERT(event == nine);
    CPPUNIT_ASSERT(updated && !gap);

    eventWriter1.push(ten);
    eventWriter1.push(eleven);

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == ten);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == ten);
    CPPUNIT_ASSERT(updated && !gap);

    updated = eventReader1.peek(event, gap);
    CPPUNIT_ASSERT(event == eleven);
    CPPUNIT_ASSERT(updated && !gap);
    updated = eventReader1.pop(event, gap);
    CPPUNIT_ASSERT(event == eleven);
    CPPUNIT_ASSERT(updated && !gap);
}
