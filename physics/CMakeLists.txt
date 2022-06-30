target_sources(projLib
        PUBLIC
        include/component.h
        include/cruise_engine.h
        include/directions.h
        include/engine.h
        include/mini_engine.h
        include/vector.h
        src/component.cpp
        src/engine.cpp
        src/vector.cpp
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})