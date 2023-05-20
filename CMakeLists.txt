cmake_minimum_required(VERSION 3.0)
project(OpenSkyAPI)

# Set the C++ standard to use
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

# Create a build directory
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Add the library target
add_library(utility SHARED utility.cpp utility.h)

# Add the executable target
add_executable(opensky openSkyAPI.cpp)

# Link the executable with the library
target_link_libraries(opensky PRIVATE utility)

# Enable C++17 features and the filesystem library
target_compile_features(utility PRIVATE cxx_std_17)
target_compile_features(opensky PRIVATE cxx_std_17)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(utility PRIVATE "-Wall" "-Wextra" "-pedantic")
    target_compile_options(opensky PRIVATE "-Wall" "-Wextra" "-pedantic")
endif()

# Link the filesystem library if needed (for GCC/Clang)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_libraries(opensky PRIVATE stdc++fs)
endif()
