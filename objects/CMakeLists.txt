target_sources(projLib
        PUBLIC
        include/asteroid.h
        include/phosphor_ghost.h
        include/player_ship.h
        include/player_ship_item.h
        include/starfield.h
        src/asteroid.cpp
        src/phosphor_ghost.cpp
        src/player_ship.cpp
        src/player_ship_item.cpp
        src/starfield.cpp
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})