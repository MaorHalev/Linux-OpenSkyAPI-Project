cmake_minimum_required(VERSION 3.10)
project(openSkyAPI)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../")

# Find libzip
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED libzip)

# Set utility source files
set(UTILITY_SRCS src/utility.cpp src/utility.h)

# Set processComunication source files
set(PROCESS_COMMUNICATION_SRCS src/processComunication.cpp src/processComunication.h)

# Create the shared library for utility
add_library(utility SHARED ${UTILITY_SRCS})

# Set the output name and properties of the shared library
set_target_properties(utility PROPERTIES OUTPUT_NAME "utility" VERSION 1.0 SOVERSION 1)

# Include directories for utility and processComunication
include_directories(${CMAKE_CURRENT_SOURCE_DIR} utility processComunication)

# Create the executable from openSkyAPI.cpp and processComunication.cpp
add_executable(openSkyAPI.out src/openSkyAPI.cpp ${PROCESS_COMMUNICATION_SRCS})

# Link the executable against the utility shared library and libzip
target_link_libraries(openSkyAPI.out utility ${LIBZIP_LIBRARIES})