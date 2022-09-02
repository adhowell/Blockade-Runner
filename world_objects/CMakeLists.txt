target_sources(projLib
        PUBLIC
        include/faction.h
        include/missile.h
        include/player_ship.h
        include/world_object.h
        src/player_ship.cpp
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})