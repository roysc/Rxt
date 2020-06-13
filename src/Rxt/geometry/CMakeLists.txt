# Rxt geometry

if(EMSCRIPTEN)
    message(WARNING "Rxt::Geometry does not support Emscripten and will not be built")
    return()
endif()

find_package(CGAL 5.0 REQUIRED)
include(${CGAL_USE_FILE})

set(_HEADERS
    surface.hpp
    shapes.hpp
    helper.hpp
    triangle_primitive.hpp
    mesh_transform.hpp
    mesh_traits.hpp
    segment_between.hpp
    util.hpp
)
set(_SOURCES
    geometry.cpp
)

add_library(RxtGeometry ${_HEADERS} ${_SOURCES})
_add_component(Geometry)

target_include_directories(RxtGeometry PUBLIC ${CGAL_INCLUDE_DIRS})
_use_boost(RxtGeometry) # using property_map

# also: CGAL_ImageIO, CGAL_3RD_PARTY_LIBRARIES
target_link_directories(RxtGeometry PUBLIC ${CGAL_LIBRARY_DIRS})
target_link_libraries(RxtGeometry ${CGAL_LIBRARIES})

# target_compile_definitions(RxtGeometry PRIVATE ${CGAL_CXX_FLAGS_DEBUG_INIT}) # XXX adds "-D-g"
# target_compile_definitions(RxtGeometry PUBLIC _LIBCPP_ENABLE_CXX17_REMOVED_FEATURES)

# make_test(surface)
make_test(shapes FILES shapes.test.cpp)
target_link_libraries(test_shapes RxtGeometry)

install(FILES ${_HEADERS}
    DESTINATION "${_INCLUDE_DEST}/Rxt/geometry"
    COMPONENT Geometry)

_install_component(Geometry)
