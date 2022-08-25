target_sources(projLib
        PUBLIC
        include/tactical_view.h
        include/config_view.h
        include/globals.h
        src/tactical_view.cpp
        src/config_view.cpp
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})