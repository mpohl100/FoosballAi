add_executable(testFoosballField testFoosballField.cpp)
target_include_directories(testFoosballField PUBLIC ../include)
target_link_libraries(testFoosballField kicker)