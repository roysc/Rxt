# Rxt meta-programming tools

set(_HEADERS
    constructor.hpp
    meta.cpp
)
add_library(RxtMeta ${_HEADERS})
use_stdlib(RxtMeta ${_STDLIB_DIR} ${_STDLIB_LIBDIR})
target_include_directories(RxtMeta PUBLIC
    $<BUILD_INTERFACE:${_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${_INCLUDE_DEST}>
)

install(FILES ${_HEADERS} DESTINATION "${_INCLUDE_DEST}/Rxt/meta")
