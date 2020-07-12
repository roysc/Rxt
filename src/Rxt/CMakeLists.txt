# CMake config for source tree
include(CMakeFindDependencyMacro)

include(RxtFunctions)

set(_LOCAL_PREFIX "$ENV{HOME}/.local")

set(_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/Rxt)
set(_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)
set(_TESTS_DIR ${_SOURCE_DIR})

if(EMSCRIPTEN)
    list(APPEND CMAKE_MODULE_PATH "${_LOCAL_PREFIX}/lib/cmake")
    list(APPEND CMAKE_PREFIX_PATH "${_LOCAL_PREFIX}/lib/cmake")
endif()

if(EMSCRIPTEN)
    # Force find_ functions to search outside Emscripten's redefined root path
    set(RXT_EMSCRIPTEN_FIND_ROOT_PATH_MODE CMAKE_FIND_ROOT_PATH_BOTH)
endif()

# Define base library
set(_HEADERS
    _debug.hpp
    error.hpp
    io.hpp
    log.hpp
    math.hpp
    range.hpp
    time.hpp
    util.hpp
    meta.hpp
    "macro.hpp"
    loader.hpp
)
set(_SOURCES
)

# Base library is installed by default; dependencies of other components should live here.
set(RXT_BASE Base)
set(RXT_BASE_LIBRARY Rxt${RXT_BASE})
set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME ${RXT_BASE})

add_library(RxtBase ${_HEADERS} ${_SOURCES})
_add_component(Base)

set_target_properties(RxtBase PROPERTIES LINKER_LANGUAGE CXX)
if(NOT DISABLE_FMT)
    find_package(fmt 6.0 CONFIG REQUIRED)
    target_link_libraries(RxtBase fmt::fmt)
endif()

install(FILES ${_HEADERS}
    DESTINATION "${_INCLUDE_DEST}/Rxt"
    COMPONENT ${RXT_BASE})

_install_component(${RXT_BASE})

# Component libs
add_subdirectory(data)
add_subdirectory(geometry)
add_subdirectory(graphics)

# Packaging
set(_config_file "${PROJECT_BINARY_DIR}/cmake/RxtConfig.cmake")
set(_version_file "${PROJECT_BINARY_DIR}/cmake/RxtConfigVersion.cmake")
set(CMAKE_INSTALL_MODULEDIR ${_CMAKE_MODULE_DEST})

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/RxtConfig.cmake.in" ${_config_file}
    INSTALL_DESTINATION ${_CMAKE_MODULE_DEST}
    PATH_VARS CMAKE_INSTALL_MODULEDIR)

write_basic_package_version_file(${_version_file}
    VERSION ${PROJECT_VERSION} COMPATIBILITY ExactVersion)

install(FILES ${_config_file} DESTINATION ${_CMAKE_MODULE_DEST})
install(FILES ${_version_file} DESTINATION ${_CMAKE_MODULE_DEST})

# Tests for base
set(_TESTS meta io log time range "macro")
foreach(_TEST ${_TESTS})
    make_test(${_TEST} FILES ${_TEST}.test.cpp LINK RxtBase)
endforeach()
_use_boost(test_range)

if(BUILD_TESTING)
    find_dependency(glm CONFIG REQUIRED ${RXT_EMSCRIPTEN_FIND_ROOT_PATH_MODE})
    make_test(math FILES math.test.cpp LINK RxtBase glm::glm)
endif()
