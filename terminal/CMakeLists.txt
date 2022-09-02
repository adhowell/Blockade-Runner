target_sources(blockadeRunnerLib
        PUBLIC
        include/history.h src/history.cpp
        include/input.h src/input.cpp
        include/terminal.h src/terminal.cpp
        )

target_include_directories(blockadeRunnerLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})