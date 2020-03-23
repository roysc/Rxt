# find_package(noise REQUIRED)

# Rxt demos
function(make_demo _NAME _SOURCE _LIBS)
    add_executable(${_NAME} ${_SOURCE})
    target_link_libraries(${_NAME} ${_LIBS})
endfunction()

find_package(Boost 1.68 REQUIRED) # using multi_array

if(BUILD_DEMOS)
    make_demo(sdl_basic easy_sdl.cpp RxtGraphics)
    make_demo(sdl_events sdl_events.cpp RxtGraphics)

    add_executable(geom_viewer geom_viewer.cpp geom_viewer.hpp)
    target_link_libraries(geom_viewer RxtGraphics RxtGeometry)

    add_executable(grid_viewer grid_viewer.cpp)
    target_link_libraries(grid_viewer RxtGraphics fmt::fmt)

    add_subdirectory(gridworld)
    add_subdirectory(gridquest)
endif()
