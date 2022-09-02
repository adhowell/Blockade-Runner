target_sources(blockadeRunnerLib
        PUBLIC
        include/asteroid.h src/asteroid.cpp
        include/phosphor_ghost.h src/phosphor_ghost.cpp
        include/player_ship_item.h src/player_ship_item.cpp
        include/starfield.h src/starfield.cpp
        )

target_include_directories(blockadeRunnerLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})