cmake_minimum_required(VERSION 3.5)
project(robot)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O0")

set(SOURCE_FILES main.cpp InstrumentedRobot.h Robot.h TemperatureMonitor.h RobotOperationOrderMonitor.h ../../Monitor.cpp ../../ArraySeqLock.cpp ../../timespecUtils.cpp)
add_executable(robot ${SOURCE_FILES})

find_library(PTHREAD pthread)

target_link_libraries(robot ${PTHREAD})