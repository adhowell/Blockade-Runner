target_sources(projLib
        PUBLIC
        include/config_view.h
        include/globals.h
        include/strategic_view.h
        include/tactical_view.h
        src/config_view.cpp
        src/strategic_view.cpp
        src/tactical_view.cpp
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})