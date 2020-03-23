# Rxt project
# top-level CMakeLists.txt
cmake_minimum_required(VERSION 3.8)
project(Rxt VERSION 0.1)

include(CTest)

function(_debug _VAR)
    message("** ${_VAR}=${${_VAR}}")
endfunction()

set(_INCLUDE_DEST "include")
set(_LIB_DEST "lib/Rxt")
set(_CMAKE_MODULE_DEST "lib/cmake/Rxt")

if(NOT Rxt_LOCAL_PREFIX)
    set(Rxt_LOCAL_PREFIX "$ENV{HOME}/.local")
endif()

# set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std=c++2a")
# set(CMAKE_CXX17_STANDARD_COMPILE_OPTION "-std=c++17")

# Global options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

option(BUILD_DEMOS "Build executable graphics demos" OFF)
# option(BUILD_TESTS "Build tests" ON)

add_subdirectory(src/Rxt)
add_subdirectory(cmake)

install(DIRECTORY data/shader/ DESTINATION ${_LIB_DEST}/shader)
