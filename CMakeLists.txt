# CMake Version Number
cmake_minimum_required(VERSION 3.5.2)

# Project Name
project(NotCrosscavern)

# Project Version Number
set(NotCrosscavern_VERSION_MAJOR 0)
set(NotCrosscavern_VERSION_MINOR 0)

# Compiler Flags
set(CMAKE_CXX_FLAGS "-std=c++11")

# Output Directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Executables
add_executable(using_SDL_general_engine ${CMAKE_SOURCE_DIR}/src/using_SDL/general_engine.cpp)
add_executable(using_SDL_movement ${CMAKE_SOURCE_DIR}/src/using_SDL/movement.cpp)
add_executable(using_SDL_test ${CMAKE_SOURCE_DIR}/src/using_SDL/test.cpp)
