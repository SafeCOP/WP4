cmake_minimum_required(VERSION 3.1) 

project(Monitor)

file(GLOB C_FRAMEWORK_SRC
        "../../lib/c/*.h"
        "../../lib/c/*.c"
	)

file(GLOB CPP_FRAMEWORK_SRC
        "../../lib/cpp/*.h"
        "../../lib/cpp/*.cpp"
	)
	
file(GLOB CPP_FACTORY_FRAMEWORK_SRC
        "../../lib/cpp/factories/*.h"
        "../../lib/cpp/factories/*.cpp"
	)

file(GLOB SRC
        "*.h"
        "*.cpp"
	)

SET_SOURCE_FILES_PROPERTIES(../../lib/c/raven_api.c PROPERTIES LANGUAGE CXX)

add_executable(Monitor ${C_FRAMEWORK_SRC} ${CPP_FRAMEWORK_SRC} ${SRC} ${CPP_FACTORY_FRAMEWORK_SRC})

find_library(PTHREAD pthread)

target_link_libraries(Monitor ${PTHREAD})
