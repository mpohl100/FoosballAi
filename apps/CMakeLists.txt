link_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(foosballAi main.cpp)
target_link_libraries(foosballAi strat)
target_include_directories(foosballAi PUBLIC ../include)