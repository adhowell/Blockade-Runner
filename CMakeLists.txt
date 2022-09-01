cmake_minimum_required(VERSION 3.22)
project(blockade_runner)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_library(projLib STATIC)
add_subdirectory(views)
add_subdirectory(physics)
add_subdirectory(effects)
add_subdirectory(objects)
add_subdirectory(terminal)
add_subdirectory(models)
add_subdirectory(world_objects)

target_link_libraries(projLib PUBLIC Qt5::Widgets)

add_executable(blockade_runner
        main.cpp main_window.cpp simulation_loop.cpp main_window.h global_config.h simulation_loop.h)

target_link_libraries(blockade_runner PUBLIC Qt5::Core Qt5::Widgets projLib)

#add_subdirectory(tests)