target_sources(projLib
        PUBLIC
        include/history.h
        include/input.h
        include/terminal.h
        src/history.cpp
        src/input.cpp
        src/terminal.cpp
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})