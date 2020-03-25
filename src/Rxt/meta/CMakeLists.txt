# Rxt meta-programming tools

set(_HEADERS
    constructor.hpp
    demangle.hpp
    meta.cpp
)
set(_SOURCES
    demangle.cpp
)
add_library(RxtMeta ${_HEADERS} ${_SOURCES})

use_stdlib(RxtMeta ${_STDLIB_DIR} ${_STDLIB_LIBDIR})
target_include_directories(RxtMeta PUBLIC
    $<BUILD_INTERFACE:${_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${_INCLUDE_DEST}>
)

make_test(demangle demangle.test.cpp)
target_link_libraries(test_demangle RxtMeta)

install(FILES ${_HEADERS} DESTINATION "${_INCLUDE_DEST}/Rxt/meta")
