# Rxt project
# top-level CMakeLists.txt

cmake_minimum_required(VERSION 3.12)
project(Rxt VERSION 0.1)

set(_INCLUDE_DEST "include")
set(_LIB_DEST "lib/Rxt")
set(_CMAKE_MODULE_DEST "lib/cmake/Rxt")

# Global options
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

option(BUILD_DEMOS "Build executable graphics demos" OFF)
option(BUILD_TESTING "Build tests" OFF)

option(DISABLE_BOOST "Purge projects requiring Boost" OFF) #todo USE_BOOST, USE_FMT
option(DISABLE_FMT "Disable all use of fmt" OFF)

option(WEBGL_COMPAT "Build with WebGL compatibility changes" OFF) #todo ENABLE_WEBGL_COMPAT

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_subdirectory(src/Rxt)
add_subdirectory(cmake)
