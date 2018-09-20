#ifndef MONITOR_MONITOR_H
#define MONITOR_MONITOR_H

#include <time.h>
#include <boost/atomic.hpp>
#include <pthread.h>

using namespace boost;

#include "IEventBuffer.h"
#include "EventBuffer.h"
#include "SynchronizedEventReader.h"

/**
 * Represents a periodic Monitor.
 *
 * This class represents a periodic monitor, capable of monitoring several EventBuffers.  The monitor creates a periodic
 * pthread that executes the code on the pure virtual function run.
 *
 * \warning
 * The monitor must always exist in memory after enabling it, failure to do so will result in undefined behavior.
 *
 * Users can create SynchronizedEventReaders by calling the configSynchronizedEventReader function.
 *
 * @author Humberto Carvalho (1120409@isep.ipp.pt)
 * @date
 */
class Monitor {
private:
    /**
     * The timestamp which will be the shared synchronization variable between this Monitors SynchronizedEventReaders.
     */
    struct timespec lastReadEvent;
    /**
     * The Monitors period.
     */
    struct timespec period;

    /**
     * The Monitors pthread.
     */
    pthread_t thread;

    /**
     * The schedule policy of this Monitor. Can be: SCHED_OTHER, SCHED_FIFO, SCHED_RR, SCHED_BATCH, SCHED_IDLE as defined in pthread.
     */
    const int schedule_policy;

    /**
     * The priority for the schedule policy, please see the pthread documentation for more information.
     */
    const int priority;

    /**
     * If true it tells the Monitor pthread to terminate.
     */
    atomic<bool> exit;

    /**
     * If true then a deadline has been missed.
     */
    atomic<bool> dead_line_miss;

    /**
     * Indicates whether the monitor is running.
     */
    atomic<bool> running;

    /**
     * True if dead line miss detection is enabled for this monitor.
     */
    bool dead_line_miss_check;

    /**
     * Receives a pointer to a monitor and executes its run function in a loop until it asynch_disable is called,
     * checking for dead line misses.
     *
     * The task will arrive with the correct period.
     *
     * @param ptr pointer to the monitor being ran.
     */
    static void *pthreadFunctionChecked(void *ptr);

    /**
    * Receives a pointer to a monitor and executes its run function in a loop until it asynch_disable is called.
    *
    * The task will arrive with the correct period.
    *
    * @param ptr pointer to the monitor being ran.
    */
    static void *pthreadFunctionUnchecked(void *ptr);


    /**
     * Creates a thread for this monitor which will run function f.
     *
     * @param f the function to start the thread with.
     */
    int enable(void *(*f)(void *));

protected:
    /**
     * Configures a SynchronizedEventReader to an IEventBuffer buffer.
     *
     * @param eventReader the EventReader to be configured.
     * @param buffer a reference to an IEventBuffer.
     */
    template<typename T>
    void configSynchronizedEventReader(SynchronizedEventReader<T> &eventReader, const IEventBuffer<T> &buffer);

    /**
     * The monitor execution code.
     *
     * Users should overwrite this function  with their monitoring code. It will be called periodically as defined by
     * the users period.
     *
     */
    virtual void run() = 0;

    /**
     * Action to execute when a deadline miss occurs.
     *
     * The default action simply prints to std err.
     */
    virtual void deadLineMissAction();

    /**
     * When called this function will quit the monitor when the run iteration finishes.
     */
    void quit();

    /**
     * Instantiates a new monitor with a period of N seconds and N nano seconds.
     *
     * The schedule policy will be SCHED_OTHER with priority 0.
     *
     * @param seconds the number of seconds of this monitors period.
     * @param nanoSeconds the number of nanoseconds of this monitors period.
     */
    Monitor(const unsigned long seconds, const unsigned long nanoSeconds);

    /**
     * Instantiates a new monitor with a period of timespec.
     *
     * The schedule policy will be SCHED_OTHER with priority 0.
     *
     * @param period the monitors period.
     */
    Monitor(const struct timespec &period);

    /**
    * Instantiates a new monitor with a period of N seconds and N nano seconds, with schedule policy schedule_policy and priority priority.
    *
    * @param seconds the number of seconds of this monitors period.
    * @param nanoSeconds the number of nanoseconds of this monitors period.
    * @param schedule_policy the posix schedule policy for this monitor.
    * @param priority the priority for this monitor, should match the schedule policy.
    */
    Monitor(const unsigned long seconds, const unsigned long nanoSeconds, int schedule_policy, int priority);

    /**
     * Instantiates a new monitor with a period of timespec, with schedule policy schedule_policy and priority priority. Can be: SCHED_OTHER, SCHED_FIFO, SCHED_RR, SCHED_BATCH, SCHED_IDLE as defined in pthread.
     *
     * @param period the monitors period.
     * @param schedule_policy the posix schedule policy for this monitor.
     * @param priority the priority for this monitor, should match the schedule policy.
     */
    Monitor(const struct timespec &period, int schedule_policy, int priority);

public:
    /**
     * Enables this Monitor with deadline checking.
     *
     * The monitor frequently checks if a dead line has been missed. If a deadline miss occurrs then it'll be logged into
     * stderr and the dead line miss bool will be set to true.
     *
     * Calling this function creates a thread that executes the run function with a period defined by the user.
     *
     * @returns 0 if success. On error -1 is returned and errno is set appropriately.
     *
     * Error codes:
     *      -  EINVAL: The argument policy does not identify a defined scheduling
     *      -  EINVAL: The priority specified in param does not make sense for the
     *         current scheduling policy of attr.
     *      -  EINVAL: An invalid value was specified in detachstate.
     *      -  EAGAIN: Insufficient resources to create another thread.
     *      -  EAGAIN A system-imposed limit on the number of threads was
     *         encountered.  There are a number of limits that may trigger
     *         this error: the RLIMIT_NPROC soft resource limit (set via
     *         setrlimit(2)), which limits the number of processes and
     *         threads for a real user ID, was reached; the kernel's system-
     *         wide limit on the number of processes and threads,
     *         /proc/sys/kernel/threads-max, was reached (see proc(5)); or
     *         the maximum number of PIDs, /proc/sys/kernel/pid_max, was
     *         reached (see proc(5)).
     *      -  EINVAL: Invalid settings in attr.
     *      -  EPERM: No permission to set the scheduling policy and parameters
     *              specified in attr.
     *      -  EALREADY: The monitor is already enabled.
     */
    int enableWithDeadlineMissCheck();

    /**
     * Enables this Monitor without deadline checking.
     *
     * Calling this function creates a thread that executes the run function with a period defined by the user.
     *
     * @returns 0 if success. On error -1 is returned and errno is set appropriately.
     *
     * Error codes:
     *      -  EINVAL: The argument policy does not identify a defined scheduling
     *      -  EINVAL: The priority specified in param does not make sense for the
     *         current scheduling policy of attr.
     *      -  EINVAL: An invalid value was specified in detachstate.
     *      -  EAGAIN: Insufficient resources to create another thread.
     *      -  EAGAIN A system-imposed limit on the number of threads was
     *         encountered.  There are a number of limits that may trigger
     *         this error: the RLIMIT_NPROC soft resource limit (set via
     *         setrlimit(2)), which limits the number of processes and
     *         threads for a real user ID, was reached; the kernel's system-
     *         wide limit on the number of processes and threads,
     *         /proc/sys/kernel/threads-max, was reached (see proc(5)); or
     *         the maximum number of PIDs, /proc/sys/kernel/pid_max, was
     *         reached (see proc(5)).
     *      -  EINVAL: Invalid settings in attr.
     *      -  EPERM: No permission to set the scheduling policy and parameters
     *              specified in attr.
     *      -  EALREADY: The monitor is already enabled.
     */
    int enable();

    /**
     * Checks whether this Monitor is running.
     *
     * @returns true if the monitor is running.
     */
    bool isRunning();

    /**
     * Atomically switches whatever value is in the dead line miss flag by false, returning
     * the value that was previously contained.
     *
     * @warning the monitor must be enabled with dead line miss checking.
     *
     * @returns the deadline flag value was there before resetting (0 for false and 1 for true), or -1 if the monitor
     * is not running or has deadline miss check dectection disabled.
     */
    char resetDeadLine();

    /**
     * Checks if a dead line has been missed.
     *
     * @warning the monitor must be enabled with dead line miss checking.
     *
     * @return 1 if a dead line has been missed, 0 if it has not, -1 if the monitor is not running or has dead line
     * miss check detection disabled.
     */
    char deadLineMiss();

    /**
     *  Asynchronously disables this monitor.
     *
     * Calling this function will set exit to true causing the thread to exit.
     */
    void asynch_disable();

    /**
     * Synchronously disables this monitor.
     *
     * Calling this function will set exit to true causing the thread to exit, busy waiting until it has.
     */
    void synch_disable();

    /**
     * Returns the Monitors period.
     * @return the monitor period.
     */
    const timespec &getPeriod();
};

template<typename T>
void Monitor::configSynchronizedEventReader(SynchronizedEventReader<T> &synchronizedEventReader,
                                            const IEventBuffer<T> &buffer) {
    buffer.configSynchronizedEventReader(&lastReadEvent, synchronizedEventReader);
}

#endif //MONITOR_MONITOR_H
