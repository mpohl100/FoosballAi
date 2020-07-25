
set(KICKER_SOURCES
        FoosballField.cpp       
        Kicker.cpp 
        Point.cpp 
        ConstrainedPosition.cpp 
        Ball.cpp 
        Figure.cpp 
        Rod.cpp
        Shot.cpp
)

add_library(kicker SHARED ${KICKER_SOURCES})
target_include_directories(kicker PUBLIC ../include)

set(STRAT_SOURCES
        OffensiveStrat.cpp
        DefensiveStrat.cpp
        StratCommon.cpp
)

add_library(strat SHARED ${STRAT_SOURCES})
target_include_directories(strat PUBLIC ../include)
target_link_libraries(strat kicker)