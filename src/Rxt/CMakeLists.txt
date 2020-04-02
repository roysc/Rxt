# Rxt source cmake

set(_STDLIB_DIR ${Rxt_LOCAL_PREFIX}/include/c++/9.2.0)
set(_STDLIB_LIBDIR ${Rxt_LOCAL_PREFIX}/lib64)

# Use correct std headers
function(use_stdlib _TGT _DIR _LIBDIR)
    if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
        target_include_directories(${_TGT} PUBLIC ${_DIR} ${_DIR}/x86_64-pc-linux-gnu)
        target_compile_options(${_TGT} PUBLIC -nostdinc++)
        target_link_options(${_TGT} PUBLIC -L${_LIBDIR} -Wl,-rpath,${_LIBDIR})
    endif()
endfunction()

add_compile_options(-Wall -Werror=return-type) # -Wpedantic -Wunused

set(_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src/Rxt)
set(_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)
set(_TESTS_DIR ${_SOURCE_DIR})

# Define base library
find_package(fmt 6.0 REQUIRED)
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
)

install(FILES ${_HEADERS} DESTINATION "${_INCLUDE_DEST}/Rxt")

# "Base" library includes fmt for debugging
add_library(RxtBase
    ${_HEADERS}
    ${_SOURCES}
)
use_stdlib(RxtBase ${_STDLIB_DIR} ${_STDLIB_LIBDIR})
target_include_directories(RxtBase PUBLIC
    $<BUILD_INTERFACE:${_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${_INCLUDE_DEST}>
)

target_link_libraries(RxtBase fmt::fmt)
# target_link_libraries(RxtBase Boost)

set(_LIBDIR ${Rxt_LOCAL_PREFIX}/lib)
target_link_options(RxtBase PUBLIC -L${_LIBDIR} -Wl,-rpath,${_LIBDIR})

# Tests
function(make_test _NAME _SOURCES)
    add_executable(test_${_NAME} ${_SOURCES})
    target_include_directories(test_${_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/external)
    add_test(NAME test_${_NAME} COMMAND ${_NAME})
endfunction()

make_test(range range.test.cpp)
target_link_libraries(test_range RxtBase)

# Component libs
add_subdirectory(data)
add_subdirectory(geometry)
add_subdirectory(graphics)
add_subdirectory(meta)

add_subdirectory(demo)

# Packaging
install(
    TARGETS RxtBase RxtData RxtGeometry RxtGraphics RxtMeta
    EXPORT Rxt
    DESTINATION ${_LIB_DEST}
)
install(EXPORT Rxt DESTINATION ${_CMAKE_MODULE_DEST})
