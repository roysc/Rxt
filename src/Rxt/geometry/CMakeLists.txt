# Rxt geometry

# set(CGAL_DIR "/usr/lib/x86_64-linux-gnu/cmake/CGAL")
# set(CGAL_DIR $ENV{HOME}/fork/CGAL/build)
find_package(CGAL REQUIRED)
include(${CGAL_USE_FILE})

set(_HEADERS
    surface.hpp
    shapes.hpp
    helper.hpp
    triangle_primitive.hpp
    # mesh_transform.hpp
    mesh_traits.hpp
    # segment_between.hpp
    constants.hpp
    util.hpp
)
add_library(RxtGeometry geometry.cpp ${_HEADERS})

use_stdlib(RxtGeometry ${_STDLIB_DIR} ${_STDLIB_LIBDIR})
target_include_directories(RxtGeometry PUBLIC
    $<BUILD_INTERFACE:${_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:${_INCLUDE_DEST}>
)
# TODO: should be private?
target_include_directories(RxtGeometry PUBLIC ${CGAL_INCLUDE_DIRS})

target_link_directories(RxtGeometry PUBLIC ${CGAL_LIBRARY_DIRS})
target_link_libraries(RxtGeometry ${CGAL_LIBRARIES})
# target_link_libraries(RxtGeometry CGAL_ImageIO)
# target_link_libraries(RxtGeometry ${CGAL_3RD_PARTY_LIBRARIES})

# target_compile_definitions(RxtGeometry PRIVATE ${CGAL_CXX_FLAGS_DEBUG_INIT}) # XXX adds "-D-g"
# target_compile_definitions(RxtGeometry PUBLIC _LIBCPP_ENABLE_CXX17_REMOVED_FEATURES)

# make_test(surface)
make_test(shapes shapes.test.cpp)
target_link_libraries(test_shapes RxtGeometry)

install(FILES ${_HEADERS} DESTINATION "${_INCLUDE_DEST}/Rxt/geometry")
