# Rxt source cmake

set(_STDLIB_DIR ${Rxt_LOCAL_PREFIX}/include/c++/9.2.0)
set(_STDLIB_LIBDIR ${Rxt_LOCAL_PREFIX}/lib64)

find_package(fmt 6.0 REQUIRED)
set(Boost_DIR ${Rxt_LOCAL_PREFIX}/include)
find_package(Boost 1.68 REQUIRED COMPONENTS graph)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 2.0.8 REQUIRED)

# Use correct std headers
function(use_stdlib _TGT _DIR _LIBDIR)
    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_include_directories(${_TGT} PUBLIC ${_DIR} ${_DIR}/x86_64-pc-linux-gnu)
        target_compile_options(${_TGT} PUBLIC -nostdinc++)
        target_link_options(${_TGT} PUBLIC -L${_LIBDIR} -Wl,-rpath,${_LIBDIR})
    endif()
endfunction()

function(_show _VAR)
    message("** ${_VAR}=${${_VAR}}")
endfunction()

add_compile_options(-Wall -Werror=return-type) # -Wpedantic -Wunused

set(_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/Rxt)
set(_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)
set(_TESTS_DIR ${_SOURCE_DIR})

set(_UTIL_HEADERS
    ${_SOURCE_DIR}/_debug.hpp
    ${_SOURCE_DIR}/io.hpp
    ${_SOURCE_DIR}/range.hpp
    ${_SOURCE_DIR}/error.hpp
    ${_SOURCE_DIR}/runtime.hpp
    ${_SOURCE_DIR}/time.hpp
    ${_SOURCE_DIR}/util.hpp
    )

install(FILES ${_UTIL_HEADERS} DESTINATION "${_INCLUDE_DEST}/Rxt")

# "Base" library includes fmt for debugging
add_library(RxtBase
    ${_UTIL_HEADERS}
    ${_SOURCE_DIR}/_debug.hpp
    )
use_stdlib(RxtBase ${_STDLIB_DIR} ${_STDLIB_LIBDIR})
target_include_directories(RxtBase PUBLIC
    $<BUILD_INTERFACE:${_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${_INCLUDE_DEST}>
)

target_link_libraries(RxtBase fmt::fmt)
set(_LIBDIR ${Rxt_LOCAL_PREFIX}/lib)
target_link_options(RxtBase PUBLIC -L${_LIBDIR} -Wl,-rpath,${_LIBDIR})

# Tests
function(make_test _NAME _SOURCES)
    add_executable(test_${_NAME} ${_SOURCES})
    target_include_directories(test_${_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/external)
    add_test(NAME test_${_NAME} COMMAND ${_NAME})
endfunction()

add_subdirectory(data)
add_subdirectory(geometry)
add_subdirectory(graphics)
add_subdirectory(meta)

add_subdirectory(demo)

# Packaging
install(TARGETS RxtBase RxtData RxtGeometry RxtGraphics RxtMeta EXPORT Rxt DESTINATION ${_LIB_DEST})
install(EXPORT Rxt DESTINATION ${_CMAKE_MODULE_DEST})
