cmake_minimum_required(VERSION 2.8.0) # Min version cmake required
Project(Hello)	# Define project

# Flag the compiler to use c++11 if possible
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Set the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Set the module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

# Set the include directory
include_directories("${CMAKE_SOURCE_DIR}/include")

#Go to CMakeLists in src
add_subdirectory(src)