#include "CppApiTest.h"
#include "../lib/cpp/EventBuffer.h"
#include "../lib/cpp/factories/EventBufferFactory.h"
#include "../lib/c/raven_api.h"


CPPUNIT_TEST_SUITE_REGISTRATION(CppApiTest);

void CppApiTest::eventReaderTest() {
    CPPUNIT_ASSERT(EventBufferFactory<int>::initialize()!=-1);

    EventBuffer<int> buffer;
    int ret= EventBufferFactory<int>::buildEventBuffer(buffer, "/dev/null", 0, 3);
    CPPUNIT_ASSERT(ret==0);

    EventWriter<int> writer;
    buffer.configureWriter(writer);

    CPPUNIT_ASSERT(writer.push(5)==0);

    EventReader<int> reader;
    buffer.configureReader(reader);
    int data;
    bool empty;
    bool gap;
    CPPUNIT_ASSERT(reader.pop(data,empty,gap)==0);
    CPPUNIT_ASSERT(!gap);
    CPPUNIT_ASSERT(!empty);
    CPPUNIT_ASSERT(data==5);

    int push_data=6;
    CPPUNIT_ASSERT(writer.push(push_data)==0);


    CPPUNIT_ASSERT(reader.peek(data,empty,gap)==0);
    CPPUNIT_ASSERT(!gap);
    CPPUNIT_ASSERT(!empty);
    CPPUNIT_ASSERT(data==6);

    CPPUNIT_ASSERT(reader.pop(data,empty,gap)==0);
    CPPUNIT_ASSERT(!gap);
    CPPUNIT_ASSERT(!empty);
    CPPUNIT_ASSERT(data==6);

    CPPUNIT_ASSERT(writer.push(7)==0);
    CPPUNIT_ASSERT(writer.push(8)==0);

    CPPUNIT_ASSERT(reader.peekHead(data,empty,gap)==0);
    CPPUNIT_ASSERT(!gap);
    CPPUNIT_ASSERT(!empty);
    CPPUNIT_ASSERT(data==8);

    CPPUNIT_ASSERT(reader.popHead(data,empty,gap)==0);
    CPPUNIT_ASSERT(!gap);
    CPPUNIT_ASSERT(!empty);
    CPPUNIT_ASSERT(data==8);

    CPPUNIT_ASSERT(reader.emptyBuffer()==1);

    CPPUNIT_ASSERT(buffer.destroy()==0);
    EventBufferFactory<int>::terminate();
}

void CppApiTest::buildEventBufferTest(){
    CPPUNIT_ASSERT(EventBufferFactory<int>::initialize()!=-1);
    EventBuffer<int> buffer;
    int ret= EventBufferFactory<int>::buildEventBuffer(buffer, "/dev/null", 0, 3);
    CPPUNIT_ASSERT(ret==0);

    ret= EventBufferFactory<int>::destroyEventBuffer(buffer);
    CPPUNIT_ASSERT(ret==0);

    ret= EventBufferFactory<int>::buildEventBuffer(buffer, 0, 3);
    CPPUNIT_ASSERT(ret==0);

    ret= EventBufferFactory<int>::destroyEventBuffer(buffer);
    CPPUNIT_ASSERT(ret==0);

    ret= EventBufferFactory<int>::buildEventBuffer(0, 3);
    CPPUNIT_ASSERT(ret==0);

    ret= EventBufferFactory<int>::destroyEventBuffer(0);
    CPPUNIT_ASSERT(ret==0);

    ret= EventBufferFactory<int>::buildEventBuffer("/dev/null",0, 3);
    CPPUNIT_ASSERT(ret==0);

    ret= EventBufferFactory<int>::destroyEventBuffer("/dev/null",0);
    CPPUNIT_ASSERT(ret==0);

    EventBufferFactory<int>::terminate();
}

void CppApiTest::eventBufferBuilderInitializeFactoryTest() {
    EventBufferFactory<int> factory;
    factory.initialize();
    EventBuffer<int> buffer;
    int ret= factory.buildEventBuffer(buffer, "/dev/null", 0, 3);
    CPPUNIT_ASSERT(ret==0);

    ret= factory.destroyEventBuffer(buffer);
    CPPUNIT_ASSERT(ret==0);

    factory.terminate();
}

void CppApiTest::synchronizedEventReaderTest() {
    CPPUNIT_ASSERT(EventBufferFactory<int>::initialize()!=-1);

    EventBuffer<int> buffer;
    int ret= EventBufferFactory<int>::buildEventBuffer(buffer, "/dev/null", 0, 3);
    CPPUNIT_ASSERT(ret==0);

    EventWriter<int> writer;
    buffer.configureWriter(writer);

    CPPUNIT_ASSERT(writer.push(5)==0);

    struct timespec synch;
    synch.tv_nsec=0; synch.tv_sec=0;
    SynchronizedEventReader<int> reader;
    buffer.configureSynchronizedReader(reader,&synch);
    int data;
    bool empty;
    bool gap;
    CPPUNIT_ASSERT(reader.pop(data,empty,gap)==0);
    CPPUNIT_ASSERT(!gap);
    CPPUNIT_ASSERT(!empty);
    CPPUNIT_ASSERT(data==5);

    int push_data=6;
    CPPUNIT_ASSERT(writer.push(push_data)==0);


    CPPUNIT_ASSERT(reader.peek(data,empty,gap)==0);
    CPPUNIT_ASSERT(!gap);
    CPPUNIT_ASSERT(!empty);
    CPPUNIT_ASSERT(data==6);

    CPPUNIT_ASSERT(reader.pop(data,empty,gap)==0);
    CPPUNIT_ASSERT(!gap);
    CPPUNIT_ASSERT(!empty);
    CPPUNIT_ASSERT(data==6);

    CPPUNIT_ASSERT(writer.push(7)==0);
    CPPUNIT_ASSERT(writer.push(8)==0);

    CPPUNIT_ASSERT(reader.peekHead(data,empty,gap)==0);
    CPPUNIT_ASSERT(!gap);
    CPPUNIT_ASSERT(!empty);
    CPPUNIT_ASSERT(data==8);

    CPPUNIT_ASSERT(reader.popHead(data,empty,gap)==0);
    CPPUNIT_ASSERT(!gap);
    CPPUNIT_ASSERT(!empty);
    CPPUNIT_ASSERT(data==8);

    CPPUNIT_ASSERT(reader.emptyBuffer()==1);

    CPPUNIT_ASSERT(buffer.destroy()==0);
    EventBufferFactory<int>::terminate();
}

void CppApiTest::acquireTest(){
    EventBufferFactory<int> factory;
    CPPUNIT_ASSERT(factory.initialize()!=-1);

    CPPUNIT_ASSERT(factory.buildEventBuffer(0,3)==0);
    CPPUNIT_ASSERT(factory.acquireEventBuffer(0)==0);

    EventBuffer<int> buffer;
    CPPUNIT_ASSERT(factory.acquireEventBuffer(buffer,0)==0);

    CPPUNIT_ASSERT(factory.destroyEventBuffer(0)==0);

    CPPUNIT_ASSERT(factory.buildEventBuffer("/dev/null",0,3)==0);
    CPPUNIT_ASSERT(factory.acquireEventBuffer("/dev/null",0)==0);
    CPPUNIT_ASSERT(factory.acquireEventBuffer(buffer,"/dev/null",0));
    CPPUNIT_ASSERT(factory.destroyEventBuffer("/dev/null",0)==0);

    CPPUNIT_ASSERT(factory.terminate()==0);
}