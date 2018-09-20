cmake_minimum_required(VERSION 3.1)
project(kernel)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

file(GLOB TESTS_SRC
        "*.h"
        "*.cpp"
)

file(GLOB TEST_SUPPORT_SRC
        "supportClasses/*.h"
        "supportClasses/*.cpp"
)

file(GLOB CAPI_SRC
        "../lib/c/*.h"
        "../lib/c/*.c"
)

file(GLOB CPPAPI_SRC
        "../lib/cpp/*.h"
        "../lib/cpp/*.cpp"
)

file(GLOB CPPAPIF_SRC
        "../lib/cpp/factories/*.h"
        "../lib/cpp/factories/*.cpp"
)

add_executable(kernel ${TESTS_SRC} ${TEST_SUPPORT_SRC} ${CAPI_SRC} ${CPPAPI_SRC} ${CPPAPIF_SRC})

SET_SOURCE_FILES_PROPERTIES(../lib/c/raven_api.c PROPERTIES LANGUAGE CXX)

find_library(CPPUNIT cppunit)

target_link_libraries(kernel ${CPPUNIT})