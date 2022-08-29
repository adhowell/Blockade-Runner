target_sources(projLib
        PUBLIC
        include/heat_flow.h
        include/radar_sensor.h
        include/sensor.h
        src/heat_flow.cpp
        src/sensor.cpp
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})