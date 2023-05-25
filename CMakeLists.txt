cmake_minimum_required(VERSION 3.10)
project(openSkyAPI)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../")

# Find libzip
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED libzip)
link_directories(${LIBZIP_LIBRARY_DIRS})

# Create the shared library
add_library(utility SHARED utility.cpp utility.h)

# Set the output name and properties of the shared library
set_target_properties(utility PROPERTIES
        OUTPUT_NAME "utility"
        VERSION 1.0
        SOVERSION 1
        )

# Include libzip headers
target_include_directories(utility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${LIBZIP_INCLUDE_DIRS})

# Build the executable from openSkyApi.cpp and link against the shared library
add_executable(openSkyAPI openSkyAPI.cpp)
target_link_libraries(openSkyAPI PRIVATE utility ${LIBZIP_LIBRARIES})

