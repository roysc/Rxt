# Rxt::Graphics component

if(EMSCRIPTEN)
    set(WEBGL_COMPAT ON)
else()
    # (Provided as ports in emscripten)
    find_package(OpenGL REQUIRED)
    find_package(SDL2 REQUIRED)
    find_package(GLEW REQUIRED)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_library(SOIL REQUIRED ${RXT_EMSCRIPTEN_FIND_ROOT_PATH_MODE})

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

    image.hpp
    camera.hpp

    # reactive.hpp
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
    target_compile_definitions(RxtGraphics PUBLIC RXT_ASSET_PATH_DEFAULT="/data")
    set(_html_shell "${PROJECT_SOURCE_DIR}/data/emscripten/shell_minimal.html")
    target_link_options(RxtGraphics PUBLIC --shell-file ${_html_shell})
else()
    target_compile_definitions(RxtGraphics PUBLIC
        RXT_ASSET_PATH_DEFAULT="$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/data>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${_LIB_DEST}>")
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
        "SHELL:-s USE_WEBGL2=1"
        "SHELL:-s USE_SDL=2"
        "SHELL:-s DISABLE_EXCEPTION_CATCHING=0"
        # "SHELL:-s CYBERDWARF=1" # not supported by the LLVM wasm backend
        # "SHELL:-s FORCE_FILESYSTEM=1"
    )

    target_compile_options(RxtGraphics PUBLIC ${_emscripten_options})
    target_link_options(RxtGraphics PUBLIC ${_emscripten_options})
    target_link_libraries(RxtGraphics
        ${RXT_BASE_LIBRARY}
        SOIL)                   #fixme
else()
    target_link_libraries(RxtGraphics PUBLIC
        Threads::Threads
        ${RXT_BASE_LIBRARY}
        SOIL
        OpenGL::GL
        ${GLEW_LIBRARIES}
        ${SDL2_LIBRARIES})

    install(DIRECTORY ${_shader_dir} DESTINATION ${_LIB_DEST}/shader)
endif()

install(FILES ${_HEADERS}
    DESTINATION "${_INCLUDE_DEST}/Rxt/graphics"
    COMPONENT Graphics)
install(FILES ${_HEADERS_SHADER}
    DESTINATION "${_INCLUDE_DEST}/Rxt/graphics/shader"
    COMPONENT Graphics)

_install_component(Graphics)

make_test(camera FILES camera.test.cpp LINK Rxt::Graphics)
make_test(sdl FILES sdl.test.cpp LINK Rxt::Graphics)
