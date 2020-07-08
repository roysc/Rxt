# Rxt geometry

if(EMSCRIPTEN)
    message(WARNING "Rxt::Geometry does not support Emscripten and will not be built")
    return()
endif()

find_package(CGAL REQUIRED) #fixme: xubuntu CGAL install doesn't use a cmake version file
if(${CGAL_VERSION} VERSION_LESS 5.0)
    message(SEND_ERROR "CGAL version ${CGAL_VERSION} not supported")
endif()
include(${CGAL_USE_FILE})

set(_HEADERS
    surface.hpp
    shapes.hpp
    helper.hpp
    triangle_primitive.hpp
    mesh_transform.hpp
    mesh_traits.hpp
    segment_between.hpp
    ray_distance.hpp
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

set(_TESTS
    shapes
    ray_distance
)
foreach(_TEST ${_TESTS})
    make_test(${_TEST} FILES ${_TEST}.test.cpp LINK RxtGeometry)
endforeach()

install(FILES ${_HEADERS}
    DESTINATION "${_INCLUDE_DEST}/Rxt/geometry"
    COMPONENT Geometry)

_install_component(Geometry)
