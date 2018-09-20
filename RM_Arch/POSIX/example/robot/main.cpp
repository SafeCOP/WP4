#include <iostream>
#include <thread>

#include "InstrumentedRobot.h"
#include "TemperatureMonitor.h"
#include "RobotOperationOrderMonitor.h"

using namespace std;

int main() {
    InstrumentedRobot robot;
    const IEventBuffer<int> &temp_event_buffer = robot.getTemp_event_buffer();

    TemperatureMonitor temperatureMonitor(&robot, temp_event_buffer);
    temperatureMonitor.enableWithDeadlineMissCheck();

    RobotOperationOrderMonitor robotOperationOrderMonitor(&robot, robot.getClean_event_buffer(),
                                                          robot.getPaint_event_buffer(), robot.getGloss_event_buffer());
    robotOperationOrderMonitor.enableWithDeadlineMissCheck();

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    for (; ;) {
        robot.paintCar();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
#pragma clang diagnostic pop
}