target_sources(projLib
        PUBLIC
        src/asteroid.cpp
        include/asteroid.h
        src/phosphor_ghost.cpp
        include/phosphor_ghost.h
        src/player_ship.cpp
        include/player_ship.h
        src/starfield.cpp
        include/starfield.h
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})