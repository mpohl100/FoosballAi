
add_library(kicker SHARED Kicker.cpp Point.cpp ConstrainedPosition.cpp Ball.cpp Figure.cpp)
target_include_directories(kicker PUBLIC ../include)

add_library(strat SHARED KickerStrat.cpp)
target_include_directories(strat PUBLIC ../include)
target_link_libraries(strat kicker)