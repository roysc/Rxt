# find_package(noise REQUIRED)

# Rxt demos
function(Rxt_demo _NAME)
    if(NOT BUILD_DEMOS)
        return()
    endif()

    set(_options) # USES_THREADS
    set(_oneValueArgs)
    set(_multiValueArgs FILES INCLUDE LINK)
    cmake_parse_arguments(_RXT_DEMO "${_options}" "${_oneValueArgs}" "${_multiValueArgs}" ${ARGN})

    add_executable(${_NAME} ${_RXT_DEMO_FILES})

    if(EMSCRIPTEN)
        set_target_properties(${_NAME} PROPERTIES SUFFIX ".html")
    elseif(WEBGL_COMPAT)
        set_target_properties(${_NAME} PROPERTIES SUFFIX "-webgl")
    endif()

    target_link_libraries(${_NAME} PUBLIC ${_RXT_DEMO_LINK})
    target_include_directories(${_NAME} PUBLIC ${_RXT_DEMO_INCLUDE})
endfunction()

Rxt_demo(sdl_basic FILES sdl_basic.cpp LINK RxtGraphics)
Rxt_demo(sdl_events FILES sdl_events.cpp LINK RxtGraphics)
Rxt_demo(sdl_shader FILES sdl_shader.cpp LINK RxtGraphics)
Rxt_demo(grid_viewer FILES grid_viewer.cpp LINK RxtGraphics)

Rxt_demo(grid_world
    FILES
      grid_world.cpp
      grid_world.hpp
      grid_space.hpp
      grid_context.hpp
    INCLUDE ${PROJECT_SOURCE_DIR}/external
    LINK RxtGraphics)
_link_boost(grid_world)

if(NOT EMSCRIPTEN)              #depends on geometry :(
    Rxt_demo(geom_viewer
        FILES geom_viewer.cpp geom_viewer.hpp
        LINK RxtGraphics RxtGeometry)
endif()
