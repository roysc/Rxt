# Rxt source cmake

set(_STDLIB_DIR ${Rxt_LOCAL_PREFIX}/include/c++/9.2.0)
set(_STDLIB_LIBDIR ${Rxt_LOCAL_PREFIX}/lib64)

# Collect a list of all registered components
set(Rxt_AVAILABLE_COMPONENTS)

function(_show _VAR)
    message("** ${_VAR}=${${_VAR}}")
endfunction()

# Use correct std headers
function(use_stdlib _TGT _DIR _LIBDIR)
    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_include_directories(${_TGT} PUBLIC ${_DIR} ${_DIR}/x86_64-pc-linux-gnu)
        target_compile_options(${_TGT} PUBLIC -nostdinc++)
        target_link_options(${_TGT} PUBLIC -L${_LIBDIR} -Wl,-rpath,${_LIBDIR})
    endif()
endfunction()

# Per-componenet package installation
function(install_component component)
    list(APPEND Rxt_AVAILABLE_COMPONENTS ${component})

    install(TARGETS Rxt${component}
        EXPORT ${component}_export COMPONENT ${component})

    install(EXPORT ${component}_export
        FILE "Rxt${component}Targets.cmake"
        NAMESPACE Rxt::
        DESTINATION ${_CMAKE_MODULE_DEST}
        COMPONENT ${component})

    set_target_properties(Rxt${component} PROPERTIES
        EXPORT_NAME ${component})

    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/Rxt${component}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/cmake/Rxt${component}Config.cmake"
        INSTALL_DESTINATION ${_CMAKE_MODULE_DEST})

    write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/cmake/Rxt${component}ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY ExactVersion)

    install(FILES "${PROJECT_BINARY_DIR}/cmake/Rxt${component}Config.cmake"
        DESTINATION ${_CMAKE_MODULE_DEST}
        COMPONENT ${component})

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Rxt${component}ConfigVersion.cmake"
        DESTINATION ${_CMAKE_MODULE_DEST}
        COMPONENT ${component})
endfunction()

add_compile_options(-Wall -Werror=return-type) # -Wpedantic -Wunused

set(_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/Rxt)
set(_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)
set(_TESTS_DIR ${_SOURCE_DIR})

# Define base library
find_package(fmt 6.0 CONFIG REQUIRED)
find_package(Boost 1.68 REQUIRED) # using iterator, range

set(_HEADERS
    ${_SOURCE_DIR}/_debug.hpp
    ${_SOURCE_DIR}/error.hpp
    ${_SOURCE_DIR}/io.hpp
    ${_SOURCE_DIR}/range.hpp
    ${_SOURCE_DIR}/runtime.hpp
    ${_SOURCE_DIR}/time.hpp
    ${_SOURCE_DIR}/util.hpp
    ${_SOURCE_DIR}/vec.hpp
    ${_SOURCE_DIR}/meta.hpp
    ${_SOURCE_DIR}/demangle.hpp
)
set(_SOURCES
    ${_SOURCE_DIR}/demangle.cpp
)

# "Base" library includes fmt for debugging
set(Rxt_BASE Base)
set(Rxt_BASE_LIBRARY Rxt${Rxt_BASE})
# Base component acts as default; all dependencies in other components should live here.
# This will be installed always, and before other components

set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME ${Rxt_BASE})

add_library(Rxt${Rxt_BASE}
    ${_HEADERS}
    ${_SOURCES}
)
add_library(Rxt::${Rxt_BASE} ALIAS Rxt${Rxt_BASE})

use_stdlib(Rxt${Rxt_BASE} ${_STDLIB_DIR} ${_STDLIB_LIBDIR})
target_include_directories(Rxt${Rxt_BASE} PUBLIC
    $<BUILD_INTERFACE:${_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${_INCLUDE_DEST}>
)

target_link_libraries(Rxt${Rxt_BASE} fmt::fmt)

install(FILES ${_HEADERS}
    DESTINATION "${_INCLUDE_DEST}/Rxt"
    COMPONENT ${Rxt_BASE})

install_component(${Rxt_BASE})

set(_LIBDIR ${Rxt_LOCAL_PREFIX}/lib)
target_link_options(Rxt${Rxt_BASE} PUBLIC -L${_LIBDIR} -Wl,-rpath,${_LIBDIR})

# Tests
function(make_test _NAME _SOURCES)
    add_executable(test_${_NAME} ${_SOURCES})
    target_include_directories(test_${_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/external)
    add_test(NAME test_${_NAME} COMMAND ${_NAME})
endfunction()

make_test(range range.test.cpp)
target_link_libraries(test_range Rxt${Rxt_BASE})

make_test(demangle demangle.test.cpp)
target_link_libraries(test_demangle Rxt${Rxt_BASE})

# Component libs
add_subdirectory(data)
add_subdirectory(geometry)
add_subdirectory(graphics)

add_subdirectory(demo)

# Packaging
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/RxtConfig.cmake.in" "${PROJECT_BINARY_DIR}/RxtConfig.cmake"
    INSTALL_DESTINATION ${_CMAKE_MODULE_DEST}
)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/RxtConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY ExactVersion
)

install(FILES "${PROJECT_BINARY_DIR}/RxtConfig.cmake" DESTINATION ${_CMAKE_MODULE_DEST})
install(FILES "${PROJECT_BINARY_DIR}/RxtConfigVersion.cmake" DESTINATION ${_CMAKE_MODULE_DEST})
