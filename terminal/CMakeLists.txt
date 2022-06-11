target_sources(projLib
        PUBLIC
        src/terminal.cpp
        include/terminal.h
        src/input.cpp
        include/input.h
        src/history.cpp
        include/history.h
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})