target_sources(blockadeRunnerLib
        PUBLIC
        include/config_view.h src/config_view.cpp
        include/globals.h
        include/strategic_view.h src/strategic_view.cpp
        include/tactical_view.h src/tactical_view.cpp
        )

target_include_directories(blockadeRunnerLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})