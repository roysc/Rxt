#pragma once

#include <CGAL/boost/graph/helpers.h>

namespace Rxt
{
template <class Mesh>
struct mesh_traits
{
    using point_property_map = typename boost::property_map<Mesh, CGAL::vertex_point_t>::type;
    using point = typename boost::property_traits<point_property_map>::value_type;
};
}
