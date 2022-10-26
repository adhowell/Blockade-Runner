target_sources(blockadeRunnerLib
        PUBLIC
        include/faction.h
        include/missile.h src/missile.cpp
        include/player_ship.h src/player_ship.cpp
        include/world_object.h
        )

target_include_directories(blockadeRunnerLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})