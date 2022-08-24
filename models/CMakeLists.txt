target_sources(projLib
        PUBLIC
        include/heat_flow.h
        src/heat_flow.cpp
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})