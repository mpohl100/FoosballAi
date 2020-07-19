cmake_minimum_required(VERSION 3.0.0)
project(foosballAi VERSION 0.1.0)

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif ()
else ()
    set (CMAKE_CXX_STANDARD 11)
endif ()

include(CTest)
enable_testing()

add_executable(foosballAi Kicker.cpp KickerStrat.cpp main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
