cmake_minimum_required(VERSION 3.1) 

project(Monitor)

file(GLOB FRAMEWORK_SRC
        "../../../lib/c/*.h"
        "../../../lib/c/*.c"
	)

file(GLOB SRC
        "*.h"
        "*.c"
	)

add_executable(Monitor ${FRAMEWORK_SRC} ${SRC})

find_library(RT rt)

target_link_libraries(Monitor ${RT})
