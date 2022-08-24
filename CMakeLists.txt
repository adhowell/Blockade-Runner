cmake_minimum_required(VERSION 3.22)
project(blockade_runner)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64/lib/cmake")
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)

add_library(projLib main.cpp main_window.cpp main_window.h global_config.h)
add_subdirectory(views)
add_subdirectory(physics)
add_subdirectory(effects)
add_subdirectory(objects)
add_subdirectory(terminal)
add_subdirectory(models)

target_link_libraries(projLib Qt5::Widgets)

add_executable(blockade_runner main.cpp main_window.cpp simulation_loop.cpp main_window.h global_config.h simulation_loop.h)

target_link_libraries(blockade_runner projLib)
target_link_libraries(blockade_runner Qt5::Core)
target_link_libraries(blockade_runner Qt5::Widgets)
target_link_libraries(blockade_runner Qt5::Quick)