target_sources(projLib
        PUBLIC
        src/vector.cpp
        src/engine.cpp
        include/vector.h
        include/engine.h
        include/cruise_engine.h
        include/mini_engine.h)

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})