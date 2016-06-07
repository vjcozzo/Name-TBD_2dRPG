cmake_minimum_required(VERSION 2.8.0) # Min version cmake required
Project(Hello)	# Define project

# Flag the compiler to use c++11 if possible
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Attach sources to the target
add_executable(Hello src/helloworld.cpp)