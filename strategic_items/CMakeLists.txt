target_sources(blockadeRunnerLib
        PUBLIC
        include/acceleration_marker.h src/acceleration_marker.cpp
        include/grid_lines.h src/grid_lines.cpp
        include/player_symbol_item.h src/player_symbol_item.cpp
        include/sensor_fov_item.h src/sensor_fov_item.cpp
        include/strategic_symbol.h src/strategic_symbol.cpp
        include/velocity_marker.h src/velocity_marker.cpp
        )

target_include_directories(blockadeRunnerLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})