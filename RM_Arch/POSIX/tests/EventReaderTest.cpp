#include "EventReaderTest.h"
#include "../EventBuffer.h"

CPPUNIT_TEST_SUITE_REGISTRATION(EventReaderTest);

void EventReaderTest::setUp() {
    TestFixture::setUp();
}

void EventReaderTest::tearDown() {
    TestFixture::tearDown();
}

void EventReaderTest::testEmpty() {
    EventBuffer<int, 3> buf;
    EventReader<int> reader;
    buf.configReader(reader);

    CPPUNIT_ASSERT(buf.empty());
    CPPUNIT_ASSERT(reader.empty());

    Event<int> event;
    bool gap = false;
    bool updated;

    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(!gap);
    CPPUNIT_ASSERT(!updated);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(!gap);
    CPPUNIT_ASSERT(!updated);

    EventWriter<int> eventWriter;
    buf.configWriter(eventWriter);
    eventWriter.push(Event<int>(1));
    CPPUNIT_ASSERT(!reader.empty());
}

void EventReaderTest::testHalfFull() {
    EventBuffer<int, 3> buf;
    EventReader<int> reader;
    buf.configReader(reader);

    Event<int> one(1);
    Event<int> two(2);

    //buffer is not empty, no exception should be thrown, should return correct element
    EventWriter<int> writer;
    buf.configWriter(writer);
    writer.push(one);

    Event<int> event;
    bool gap = false;
    bool updated;

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(event == one);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(event == one);


    //buffer is not empty, but the reader has read all the elements,
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(!updated && !gap);

    //reader is no longer empty, no exception should be thrown
    writer.push(two);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && !gap);
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
    EventBuffer<int, 3> buf;
    EventWriter<int> writer;
    buf.configWriter(writer);
    EventReader<int> reader;
    buf.configReader(reader);

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> four(4);

    writer.push(one);
    Event<int> event;
    bool gap = false;
    bool updated;

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);

    writer.push(two);
    writer.push(three);
    writer.push(four);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && !gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);

    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);

    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);


    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(!updated && !gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(!updated && !gap);
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
    EventBuffer<int, 3> buf;
    EventWriter<int> writer;
    buf.configWriter(writer);
    EventReader<int> reader;
    buf.configReader(reader);
    CPPUNIT_ASSERT(reader.empty());

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> four(4);

    writer.push(one);
    CPPUNIT_ASSERT(!reader.empty());
    writer.push(two);
    writer.push(three);
    writer.push(four);

    Event<int> event;
    bool gap = false;
    bool updated;

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);

    //buffer is now empty
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(!updated && !gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(!updated && !gap);
}

/*
 * Buffer has elements 1,2,3 written to it, reader reads element nr1.
 * We then write 4 and 5, resulting in overflow. Exception should be thrown.
 *    |
 * [3,1,2] last read=1
 * [3,4,5] last read=1, last overwritten=2
 */
void EventReaderTest::testFullThree() {
    EventBuffer<int, 3> buf;
    EventWriter<int> writer;
    buf.configWriter(writer);
    EventReader<int> reader;
    buf.configReader(reader);

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> four(4);
    Event<int> five(5);

    writer.push(one);

    Event<int> event;
    bool updated;
    bool gap;

    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);

    writer.push(two);
    writer.push(three);
    writer.push(four);
    writer.push(five);
    CPPUNIT_ASSERT(!reader.empty());


    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && gap);

    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(updated && !gap);


    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(!updated && !gap);

    CPPUNIT_ASSERT(reader.empty());

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(!updated && !gap);
}

/*
 * Buffer has elements 1,2,3 written to it, reader reads up to element nr2.
 * We then write 3,4 and 5, resulting in no overflow.
 *      |
 * [3,1,2] last read=2
 * [3,4,5] last read=2, last overwritten=2
 */
void EventReaderTest::testFullFour() {
    EventBuffer<int, 3> buf;
    EventWriter<int> writer;
    buf.configWriter(writer);
    EventReader<int> reader;
    buf.configReader(reader);

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> four(4);
    Event<int> five(5);

    writer.push(one);

    Event<int> event;
    bool updated;
    bool gap;

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);

    writer.push(two);
    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && !gap);


    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(!updated && !gap);

    CPPUNIT_ASSERT(reader.empty());


    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(!updated && !gap);

    writer.push(three);
    writer.push(four);
    writer.push(five);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(updated && !gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(updated && !gap);

    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(!updated && !gap);

    CPPUNIT_ASSERT(reader.empty());

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == five);
    CPPUNIT_ASSERT(!updated && !gap);

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
    EventBuffer<int, 3> buf;
    EventWriter<int> writer;
    buf.configWriter(writer);
    EventReader<int> reader;
    buf.configReader(reader);

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
    Event<int> twelve(12);

    writer.push(one);
    writer.push(two);
    writer.push(three);

    Event<int> event;
    bool updated;
    bool gap;

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == two);
    CPPUNIT_ASSERT(updated && !gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(updated && !gap);

    CPPUNIT_ASSERT(reader.empty());

    //empty
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == three);
    CPPUNIT_ASSERT(!updated && !gap);


    writer.push(four);
    writer.push(five);
    writer.push(six);
    writer.push(seven);
    writer.push(eight);
    writer.push(nine);
    writer.push(ten);
    writer.push(eleven);
    writer.push(twelve);

    //overflow
    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == ten);
    CPPUNIT_ASSERT(updated && gap);

    //overflow
    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == ten);
    CPPUNIT_ASSERT(updated && gap);

    //overflow
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == ten);
    CPPUNIT_ASSERT(updated && gap);

    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == eleven);
    CPPUNIT_ASSERT(updated == !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == eleven);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == twelve);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == twelve);
    CPPUNIT_ASSERT(updated && !gap);

    CPPUNIT_ASSERT(reader.empty());

    //empty
    updated = reader.pop(event, gap);
    CPPUNIT_ASSERT(event == twelve);
    CPPUNIT_ASSERT(!updated && !gap);

    CPPUNIT_ASSERT(reader.empty());

    //empty
    updated = reader.peek(event, gap);
    CPPUNIT_ASSERT(event == twelve);
    CPPUNIT_ASSERT(!updated && !gap);
}

void EventReaderTest::testPopPeekHead() {
    EventBuffer<int, 3> buf;
    EventWriter<int> writer;
    buf.configWriter(writer);
    EventReader<int> reader;
    buf.configReader(reader);

    Event<int> event;
    bool updated;
    bool gap;

    //empty
    updated = reader.popHead(event, gap);
    CPPUNIT_ASSERT(!updated && !gap);
    updated = reader.peekHead(event, gap);
    CPPUNIT_ASSERT(!updated && !gap);

    Event<int> one(1);
    Event<int> two(2);
    Event<int> three(3);
    Event<int> four(4);

    writer.push(one);
    updated = reader.peekHead(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.popHead(event, gap);
    CPPUNIT_ASSERT(event == one);
    CPPUNIT_ASSERT(updated && !gap);

    writer.push(two);
    writer.push(three);
    writer.push(four);

    updated = reader.peekHead(event, gap);
    CPPUNIT_ASSERT(four == event);
    CPPUNIT_ASSERT(updated && !gap);
    updated = reader.popHead(event, gap);
    CPPUNIT_ASSERT(updated && !gap);
    CPPUNIT_ASSERT(four == event);

    //empty
    updated = reader.popHead(event, gap);
    CPPUNIT_ASSERT(event == four);
    CPPUNIT_ASSERT(!updated && !gap);


    Event<int> five(5);
    Event<int> six(6);
    Event<int> seven(7);
    Event<int> eight(8);
    writer.push(five);
    writer.push(six);
    writer.push(seven);
    writer.push(eight);

    updated = reader.popHead(event, gap);
    CPPUNIT_ASSERT(event == eight);
    CPPUNIT_ASSERT(updated && gap);

    Event<int> nine(9);
    Event<int> ten(10);
    Event<int> eleven(11);
    Event<int> twelve(12);
    writer.push(nine);
    writer.push(ten);
    writer.push(eleven);
    writer.push(twelve);

    updated = reader.peekHead(event, gap);
    CPPUNIT_ASSERT(twelve == event);
    CPPUNIT_ASSERT(gap && updated);
}
