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

# Set containerComunication source files
set(CONTAINER_COMMUNICATION_SRCS src/containerComunication.cpp src/containerComunication.h)

# Set dbServiceLogic source files
set(DB_SERVUCE_LOGIC_SRCS src/dbServiceLogic.cpp src/dbServiceLogic.h)

# Create the shared library for utility
add_library(utility SHARED ${UTILITY_SRCS})

# Set the output name and properties of the shared library
set_target_properties(utility PROPERTIES OUTPUT_NAME "utility" VERSION 1.0 SOVERSION 1)

# Include directories for utility and containerComunication
include_directories(${CMAKE_CURRENT_SOURCE_DIR} utility containerComunication)

# Create the executable from flightService.cpp and containerComunication.cpp
add_executable(dbService.out src/dbService.cpp ${CONTAINER_COMMUNICATION_SRCS} ${DB_SERVUCE_LOGIC_SRCS})

# Create the executable from flightService.cpp and containerComunication.cpp
add_executable(flightService.out src/flightService.cpp ${CONTAINER_COMMUNICATION_SRCS} ${DB_SERVUCE_LOGIC_SRCS})

# Link the executable against the utility shared library and libzip
target_link_libraries(dbService.out utility ${LIBZIP_LIBRARIES})

# Link the executable against the utility shared library and libzip
target_link_libraries(flightService.out utility ${LIBZIP_LIBRARIES})