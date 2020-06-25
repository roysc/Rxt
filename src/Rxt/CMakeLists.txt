# CMake config for source tree

include(CTest)
include(CMakePackageConfigHelpers)
include(CMakePrintHelpers)

include(RxtEmscriptenHelpers)

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

# Collect a list of all registered components
set(RXT_AVAILABLE_COMPONENTS)

# Use correct std headers - FIXME better way to do it?
function(_use_libpath _TGT _LIBPATH_PREFIX)
    if(EMSCRIPTEN)
        return()
    endif()

    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        set(_DIR ${_LIBPATH_PREFIX}/include/c++/9.2.0)
        set(_LIBDIR ${_LIBPATH_PREFIX}/lib64)
        target_include_directories(${_TGT} PUBLIC ${_DIR} ${_DIR}/x86_64-pc-linux-gnu)
        target_compile_options(${_TGT} PUBLIC -nostdinc++)
        target_link_options(${_TGT} PUBLIC -L${_LIBDIR} -Wl,-rpath,${_LIBDIR})
    endif()

    # FIXME better way?
    target_link_options(Rxt${_COMP} PUBLIC -L${_LIBPATH_PREFIX}/lib -Wl,-rpath,${_LIBPATH_PREFIX}/lib)
endfunction()

function(_add_component _COMP)
    add_library(Rxt::${_COMP} ALIAS Rxt${_COMP})

    target_include_directories(Rxt${_COMP} PUBLIC
        $<BUILD_INTERFACE:${_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${_INCLUDE_DEST}>)

    if(DISABLE_BOOST)
        target_compile_definitions(Rxt${_COMP} PUBLIC RXT_DISABLE_BOOST)
    endif()
    if(DISABLE_FMT)
        target_compile_definitions(Rxt${_COMP} PUBLIC RXT_DISABLE_FMT)
    endif()

    _use_libpath(Rxt${_COMP} ${_LOCAL_PREFIX})
endfunction()

# Per-componenet package installation
function(_install_component _COMP)
    list(APPEND RXT_AVAILABLE_COMPONENTS ${_COMP})

    install(TARGETS Rxt${_COMP}
        EXPORT ${_COMP}_export COMPONENT ${_COMP})

    install(EXPORT ${_COMP}_export
        FILE "Rxt${_COMP}Targets.cmake"
        NAMESPACE Rxt::
        DESTINATION ${_CMAKE_MODULE_DEST}
        COMPONENT ${_COMP})

    set_target_properties(Rxt${_COMP} PROPERTIES
        EXPORT_NAME ${_COMP})

    set(_config_file "${PROJECT_BINARY_DIR}/cmake/Rxt${_COMP}Config.cmake")
    set(_version_file "${PROJECT_BINARY_DIR}/cmake/Rxt${_COMP}ConfigVersion.cmake")

    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/Rxt${_COMP}Config.cmake.in" ${_config_file}
        INSTALL_DESTINATION ${_CMAKE_MODULE_DEST})
    install(FILES ${_config_file}
        DESTINATION ${_CMAKE_MODULE_DEST} COMPONENT ${_COMP})

    write_basic_package_version_file(${_version_file}
        VERSION ${PROJECT_VERSION} COMPATIBILITY ExactVersion)
    install(FILES ${_version_file}
        DESTINATION ${_CMAKE_MODULE_DEST} COMPONENT ${_COMP})
endfunction()

function(_use_boost _TARGET)
    if(DISABLE_BOOST)
        message(SEND_ERROR "Target ${_TARGET} requires Boost, but it is disabled")
    endif()
    Rxt_emscripten_use_boost(${_TARGET})
endfunction()

add_compile_options(-Wall -Werror=return-type) # -Wpedantic -Wunused
if(EMSCRIPTEN)
    add_compile_options(-fno-diagnostics-color)
endif()

# Define base library
set(_HEADERS
    ${_SOURCE_DIR}/_debug.hpp
    ${_SOURCE_DIR}/error.hpp
    ${_SOURCE_DIR}/io.hpp
    ${_SOURCE_DIR}/math.hpp
    ${_SOURCE_DIR}/range.hpp
    ${_SOURCE_DIR}/time.hpp
    ${_SOURCE_DIR}/util.hpp
    ${_SOURCE_DIR}/meta.hpp
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

# Tests
function(make_test _NAME)
    set(_options USES_BOOST)
    set(_oneValueArgs)
    set(_multiValueArgs FILES INCLUDE LINK)
    cmake_parse_arguments(_TEST "${_options}" "${_oneValueArgs}" "${_multiValueArgs}" ${ARGN})

    add_executable(test_${_NAME} ${_TEST_FILES})
    add_test(NAME test_${_NAME} COMMAND ${_NAME})

    target_link_libraries(test_${_NAME} PUBLIC ${_TEST_LINK})
    target_include_directories(test_${_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/external)
    target_include_directories(test_${_NAME} PUBLIC ${_TEST_INCLUDE})
    set_target_properties(test_${_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test)
endfunction()

foreach(_TEST meta io time range)
    make_test(${_TEST} FILES ${_TEST}.test.cpp LINK RxtBase)
endforeach()
_use_boost(test_range)

# Component libs
add_subdirectory(data)
add_subdirectory(geometry)
add_subdirectory(graphics)

# Packaging
set(_config_file "${PROJECT_BINARY_DIR}/cmake/RxtConfig.cmake")
set(_version_file "${PROJECT_BINARY_DIR}/cmake/RxtVersionConfig.cmake")
set(CMAKE_INSTALL_MODULEDIR ${_CMAKE_MODULE_DEST})

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/RxtConfig.cmake.in" ${_config_file}
    INSTALL_DESTINATION ${_CMAKE_MODULE_DEST}
    PATH_VARS CMAKE_INSTALL_MODULEDIR)

write_basic_package_version_file(${_version_file}
    VERSION ${PROJECT_VERSION} COMPATIBILITY ExactVersion)

install(FILES ${_config_file} DESTINATION ${_CMAKE_MODULE_DEST})
install(FILES ${_version_file} DESTINATION ${_CMAKE_MODULE_DEST})
