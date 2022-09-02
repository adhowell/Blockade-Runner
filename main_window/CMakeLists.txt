target_sources(blockadeRunnerLib
        PUBLIC
        include/global_config.h
        include/main_window.h src/main_window.cpp
        include/simulation_loop.h src/simulation_loop.cpp
        )

target_include_directories(blockadeRunnerLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})