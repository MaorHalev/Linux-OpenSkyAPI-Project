cmake_minimum_required(VERSION 3.10)
project(openSkyAPI)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Create the shared library
add_library(utility SHARED utility.cpp)

# Set the output name and properties of the shared library
set_target_properties(utility PROPERTIES
        OUTPUT_NAME "utility"
        VERSION 1.0
        SOVERSION 1
        )

# Add the include directory for utility.h
target_include_directories(utility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Build the executable from openSkyApi.cpp and link against the shared library
add_executable(openSkyAPI openSkyAPI.cpp)
target_link_libraries(openSkyAPI utility)
