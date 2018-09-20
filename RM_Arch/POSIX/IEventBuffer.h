#ifndef MONITOR_IEVENTBUFFER_H
#define MONITOR_IEVENTBUFFER_H

#include "EventWriter.h"
#include "EventReader.h"
#include "SynchronizedEventReader.h"

/**
 * IEventBuffer declares an interface for the EventBuffer class.
 *
 * The main reason this class exists is to decouple readers and writers from having to use the exact template parameter
 * N when passing the EventBuffer, for example if one declared the following:
 *
 * \code
 *  EventBuffer<int, 5>
 * \endcode
 *
 * He would have to use the exact same template size_n parameter 5 everywhere in the code. If the author wanted to
 * change the EventBuffer size he would have to change every declaration in the code.
 *
 * Thus authors using the class should always use the IEventBuffer interface where possible.
 *
 * @see EventBuffer
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
template<typename T>
class IEventBuffer {
public:
    /**
    * Configures an EventWriter for this EventBuffer.
    *
    * Configures an EventWriter to write to this buffer, only one EventWriter for each Buffer may exist, so a check
    * that a writer has not been configured before is done. A writer is only created if no writer has been created
    * before.
    *
    * @param eventWriter an EventWriter to configure.
    * @return true if the writer was configured.
    *
    * @see EventWriter
    */
    virtual bool configWriter(EventWriter<T> &eventWriter) = 0;

    /**
     * Configures a new EventReader for this buffer.
     *
     * @param eventReader the EventReader to configure.
     *
     * @see EventReader
     */
    virtual void configReader(EventReader<T> &eventReader) const = 0;

    /**
     * Configures a new SynchronizedEventReader with afterTime as a synchronization variable.
     *
     * @param afterTime the synchronization variable to use with this SynchronizedEventReader. This variable is used to
     * synchronize several SynchronizedEventReaders so that each one pops elements that are newer than afterTime.
     * @param eventReader the SynchronizedEventReader to configure.
     *
     * @see SynchronizedEventReader
     * @see Monitor
     */
    virtual void configSynchronizedEventReader(struct timespec *const &afterTime,
                                               SynchronizedEventReader<T> &eventReader) const = 0;

    /**
     * Gets the EventBuffers length.
     *
     * Get length returns template parameter N, thus it is very efficient as the compiler should inline N.
     *
     * @return the EventBuffer length.
     */
    virtual size_t getLength() const = 0;

    /**
     * Checks if the buffer is empty.
     *
     * @return true if the buffer is empty.
     */
    virtual bool empty() const = 0;
};


#endif //MONITOR_IEVENTBUFFER_H
