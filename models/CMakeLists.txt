target_sources(projLib
        PUBLIC
        include/heat_flow.h
        include/radar_sensor.h
        include/sensor.h
        include/signal_track_processor.h
        src/heat_flow.cpp
        src/sensor.cpp
        src/signal_track_processor.cpp
        )

target_include_directories(projLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})