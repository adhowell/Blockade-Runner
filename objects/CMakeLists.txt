target_sources(projLib
        PUBLIC
        include/asteroid.h
        include/menu_grid_box.h
        include/phosphor_ghost.h
        include/player_ship.h
        include/player_ship_item.h
        include/starfield.h
        include/player_symbol_item.h
        src/asteroid.cpp
        src/phosphor_ghost.cpp
        src/player_ship.cpp
        src/player_ship_item.cpp
        src/starfield.cpp
        src/player_symbol_item.cpp
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})