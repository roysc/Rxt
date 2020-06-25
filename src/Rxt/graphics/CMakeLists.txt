# Rxt::Graphics component

if(EMSCRIPTEN)
    set(WEBGL_COMPAT ON)
else()
    # Provided as ports
    find_package(OpenGL REQUIRED)
    find_package(SDL2 2.0.8 REQUIRED)
    find_package(GLEW 1.13 REQUIRED)
endif()

find_package(glm REQUIRED ${RXT_EMSCRIPTEN_FIND_ROOT_PATH_MODE})

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
    sdl_metronome.hpp
    sdl_input.hpp
    sdl_input.cpp
    _sdl_debug.hpp
    sdl.hpp

    glm.hpp
    image.hpp
    camera.hpp
    color.hpp
)

set(_HEADERS_SHADER
    shader/texture_quad_2D.hpp
    shader/grid_quad_2D.hpp
    shader/colored_triangle_3D.hpp
    shader/solid_color_3D.hpp
    shader/webgl_grid_quad_2D.hpp
)

set(_SOURCES
    gl_loader.cpp
)

add_library(RxtGraphics ${_HEADERS} ${_HEADERS_SHADER} ${_SOURCES})
_add_component(Graphics)

# Point to shader sources
if(EMSCRIPTEN)
    target_compile_definitions(RxtGraphics PUBLIC RXT_GRAPHICS_DATA_DEFAULT="/data")
    set(_html_shell "${PROJECT_SOURCE_DIR}/data/emscripten/shell_minimal.html")
    target_link_options(RxtGraphics PUBLIC --shell-file ${_html_shell})
else()
    target_compile_definitions(RxtGraphics PUBLIC
        RXT_GRAPHICS_DATA_DEFAULT="$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/data>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${_LIB_DEST}>")
endif()

# For testing WebGL compatibility
if(WEBGL_COMPAT)
    message(AUTHOR_WARNING "Building in WebGL compatibility mode")
    target_compile_definitions(RxtGraphics PUBLIC RXT_WEBGL2)
endif()

target_include_directories(RxtGraphics PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external>)

set(_shader_dir "${PROJECT_SOURCE_DIR}/data/shader/")

if(EMSCRIPTEN)
    set(_emscripten_options
        --emrun # needed?
        --preload-file "${_shader_dir}@/data/shader"
        # "SHELL:-s CYBERDWARF=1" # not supported by the LLVM wasm backend
        "SHELL:-s USE_WEBGL2=1"
        "SHELL:-s USE_SDL=2"
        # "SHELL:-s FORCE_FILESYSTEM=1"
        "SHELL:-s DISABLE_EXCEPTION_CATCHING=0")

    target_compile_options(RxtGraphics PUBLIC ${_emscripten_options})
    target_link_options(RxtGraphics PUBLIC ${_emscripten_options})
    target_link_libraries(RxtGraphics
        ${RXT_BASE_LIBRARY}
        glm)
else()
    target_link_libraries(RxtGraphics PUBLIC
        ${RXT_BASE_LIBRARY}
        OpenGL::GL
        ${GLEW_LIBRARIES}
        glm
        SDL2::SDL2)

    install(DIRECTORY ${_shader_dir} DESTINATION ${_LIB_DEST}/shader)
endif()

install(FILES ${_HEADERS}
    DESTINATION "${_INCLUDE_DEST}/Rxt/graphics"
    COMPONENT Graphics)
install(FILES ${_HEADERS_SHADER}
    DESTINATION "${_INCLUDE_DEST}/Rxt/graphics/shader"
    COMPONENT Graphics)

_install_component(Graphics)
