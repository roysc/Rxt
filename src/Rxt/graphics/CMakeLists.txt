# Rxt graphics

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 2.0.8 REQUIRED)
find_package(glm REQUIRED)

set(_HEADERS
    gl_core.hpp
    gl_core.cpp
    gl_data.hpp
    gl_guard.hpp
    gl_handy.hpp
    gl_handy.cpp
    gl_loader.hpp
    _gl_debug.hpp
    gl.hpp

    sdl_core.hpp
    sdl_gl.hpp
    sdl_gui.hpp
    sdl_events.hpp
    sdl_input.hpp
    sdl_input.cpp
    _sdl_debug.hpp
    sdl.hpp

    glm.hpp
    images.hpp
    camera.hpp
    color.hpp
)
set(_HEADERS_SHADER
    shader/texture_quad_2D.hpp
    shader/grid_quad_2D.hpp
    shader/colored_triangle_3D.hpp
)

set(_SOURCES
    gl_loader.cpp
)

add_library(RxtGraphics ${_HEADERS} ${_HEADERS_SHADER} ${_SOURCES})

# target_compile_definitions(RxtGraphics PUBLIC RXT_GRAPHICS_PATH_DEFAULT "\"${PROJECT_SOURCE_DIR}\"")
target_compile_definitions(RxtGraphics PUBLIC
    RXT_GRAPHICS_DATA_DEFAULT="$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/data>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${_LIB_DEST}>"
)

use_stdlib(RxtGraphics ${_STDLIB_DIR} ${_STDLIB_LIBDIR})
target_include_directories(RxtGraphics PUBLIC
    $<BUILD_INTERFACE:${_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${_INCLUDE_DEST}>
)

target_include_directories(RxtGraphics PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external>)

target_link_libraries(RxtGraphics
    RxtBase
    RxtMeta
    OpenGL::GL
    ${GLEW_LIBRARIES}
    glm
    SDL2::SDL2
)

install(FILES ${_HEADERS} DESTINATION "${_INCLUDE_DEST}/Rxt/graphics")
install(FILES ${_HEADERS_SHADER} DESTINATION "${_INCLUDE_DEST}/Rxt/graphics/shader")
