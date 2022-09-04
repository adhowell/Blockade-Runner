target_sources(blockadeRunnerLib
        PUBLIC
        include/heat_flow.h src/heat_flow.cpp
        include/radar_sensor.h
        include/rotation_controller.h src/rotation_controller.cpp
        include/sensor.h src/sensor.cpp
        include/signal_track_processor.h src/signal_track_processor.cpp
        )

target_include_directories(blockadeRunnerLib PUBLIC ${CMAKE_CURRENT_LIST_DIR})