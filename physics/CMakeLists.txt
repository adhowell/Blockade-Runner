target_sources(blockadeRunnerLib
        PUBLIC
        include/component.h src/component.cpp
        include/cruise_engine.h
        include/directions.h
        include/engine.h src/engine.cpp
        include/mini_engine.h
        include/vector.h src/vector.cpp
        )

target_include_directories(blockadeRunnerLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})