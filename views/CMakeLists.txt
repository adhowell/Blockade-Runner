target_sources(projLib
        PUBLIC
        include/tactical_view.h
        src/tactical_view.cpp
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})