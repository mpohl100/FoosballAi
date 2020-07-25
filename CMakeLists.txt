cmake_minimum_required(VERSION 3.0.0)
project(foosballAi VERSION 0.1.0)


if( NOT CMAKE_BUILD_TYPE )
     set( CMAKE_BUILD_TYPE Release )
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set( CMAKE_CXX_FLAGS_RELEASE "-O2")
set( CMAKE_CXX_FLAGS_DEBUG "-g") 

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
endif ()
else ()
    set (CMAKE_CXX_STANDARD 11)
endif ()

include(CTest)
enable_testing()

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
