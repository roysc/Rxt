# Rxt graphics

find_package(glm REQUIRED)

set(_HEADERS
    gl_data.hpp
    gl_guard.hpp
    gl_handy.hpp
    gl_handy.cpp
    _gl_debug.hpp
    gl.hpp
    gl.cpp
    sdl_prelude.hpp
    sdl_gl.hpp
    sdl_gui.hpp
    sdl_events.hpp
    sdl_input.hpp
    sdl_input.cpp
    _sdl_debug.hpp
    sdl.hpp
    loader.hpp
    glm.hpp
    images.hpp
    color.hpp
)
set(_HEADERS_SHADER
    shader/texture_quad.hpp
    shader/grid_quad.hpp
)
add_library(RxtGraphics ${_HEADERS} ${_HEADERS_SHADER})

# target_compile_definitions(RxtGraphics PUBLIC RXT_GRAPHICS_PATH_DEFAULT "\"${PROJECT_SOURCE_DIR}\"")
target_compile_definitions(RxtGraphics PUBLIC
    RXT_GRAPHICS_PATH_DEFAULT="$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/data>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${_LIB_DEST}>"
    )

use_stdlib(RxtGraphics ${_STDLIB_DIR} ${_STDLIB_LIBDIR})
target_include_directories(RxtGraphics PUBLIC
    $<BUILD_INTERFACE:${_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${_INCLUDE_DEST}>
)

target_include_directories(RxtGraphics PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external>)

target_link_libraries(RxtGraphics
    RxtBase
    OpenGL::GL
    ${GLEW_LIBRARIES}
    SDL2::SDL2
    glm
    RxtMeta)

install(FILES ${_HEADERS} DESTINATION "${_INCLUDE_DEST}/Rxt/graphics")
install(FILES ${_HEADERS_SHADER} DESTINATION "${_INCLUDE_DEST}/Rxt/graphics/shader")
