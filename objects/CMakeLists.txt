target_sources(projLib
        PUBLIC
        include/asteroid.h
        include/grid_lines.h
        include/menu_grid_box.h
        include/phosphor_ghost.h
        include/player_ship_item.h
        include/player_symbol_item.h
        include/sensor_fov_item.h
        include/starfield.h
        include/velocity_marker.h
        src/asteroid.cpp
        src/grid_lines.cpp
        src/phosphor_ghost.cpp
        src/player_ship_item.cpp
        src/player_symbol_item.cpp
        src/sensor_fov_item.cpp
        src/starfield.cpp
        src/velocity_marker.cpp
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})