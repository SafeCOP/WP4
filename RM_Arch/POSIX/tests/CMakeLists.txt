cmake_minimum_required(VERSION 3.1) 

project(Monitor) 

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") 

file(GLOB FRAMEWORK_SRC
        "../*.h"
        "../*.cpp" 
	)
file(GLOB TESTS_SRC
        "*.h"
        "*.cpp"
    )
file(GLOB TEST_SUPPORT_SRC
        "supportClasses/*.h"
        "supportClasses/*.cpp"
	)

add_executable(Monitor ${FRAMEWORK_SRC} ${TESTS_SRC} ${TEST_SUPPORT_SRC})

find_library(CPPUNIT cppunit)
find_library(PTHREAD pthread)

target_link_libraries(Monitor ${CPPUNIT} ${PTHREAD})
