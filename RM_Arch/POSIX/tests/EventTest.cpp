#include "EventTest.h"
#include "../Event.h"

CPPUNIT_TEST_SUITE_REGISTRATION(EventTest);

void EventTest::setUp() {
    TestFixture::setUp();
}

void EventTest::tearDown() {
    TestFixture::tearDown();
}

void EventTest::testAfter() {
    Event<std::string> event("test1");
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    Event<std::string> event2("test2", spec);
    struct timespec clock;
    clock_gettime(CLOCK_REALTIME, &clock);

    CPPUNIT_ASSERT(event < event2);
    CPPUNIT_ASSERT(event <= event2);
    CPPUNIT_ASSERT(event2 < clock);
    CPPUNIT_ASSERT(event2 <= clock);
    CPPUNIT_ASSERT(event < clock);
    CPPUNIT_ASSERT(event <= clock);
    CPPUNIT_ASSERT(clock > event);
    CPPUNIT_ASSERT(clock > event2);
    CPPUNIT_ASSERT(clock >= event);
    CPPUNIT_ASSERT(clock >= event2);
}

void EventTest::testBefore() {
    struct timespec clock;
    clock_gettime(CLOCK_REALTIME, &clock);
    Event<std::string> event("test1", clock);
    struct timespec clock2;
    clock_gettime(CLOCK_REALTIME, &clock2);
    Event<std::string> event2("test2", clock2);

    CPPUNIT_ASSERT(event2 > event);
    CPPUNIT_ASSERT(event2 >= event);
    CPPUNIT_ASSERT(event == clock);
    CPPUNIT_ASSERT(event >= clock);
    CPPUNIT_ASSERT(event <= clock);
    CPPUNIT_ASSERT(event2 > clock);
    CPPUNIT_ASSERT(clock == event);
    CPPUNIT_ASSERT(clock < event2);
}

void EventTest::testEqual() {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    Event<int> a(25, time);
    Event<int> b(25, time);

    CPPUNIT_ASSERT(a == b);
    CPPUNIT_ASSERT(!(a != b));

    CPPUNIT_ASSERT(a == time);
    CPPUNIT_ASSERT(a >= time);
    CPPUNIT_ASSERT(b == time);
    CPPUNIT_ASSERT(b <= time);

    CPPUNIT_ASSERT(time == a);
    CPPUNIT_ASSERT(time == b);

    struct timespec clock2;
    clock_gettime(CLOCK_REALTIME, &clock2);
    Event<int> c(35, clock2);
    CPPUNIT_ASSERT(!(a == c));
    CPPUNIT_ASSERT(c != time);
    CPPUNIT_ASSERT(time != c);

}

void EventTest::testDiferent() {
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    Event<int> a(25, time);
    Event<int> b(25, time);

    CPPUNIT_ASSERT(!(a != b));

    Event<int> c(25, time);
    CPPUNIT_ASSERT(a == c);
    CPPUNIT_ASSERT(!(c != time));

    struct timespec clock;
    clock_gettime(CLOCK_REALTIME, &clock);
    CPPUNIT_ASSERT(c != clock);
    CPPUNIT_ASSERT(!(time != c));
}
