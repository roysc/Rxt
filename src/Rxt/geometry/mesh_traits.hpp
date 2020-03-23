#pragma once

#include <CGAL/Surface_mesh.h>
#include <CGAL/Point_3.h>

namespace Rxt
{
template <class Mesh>
struct mesh_traits;

template <class K>
struct mesh_traits<CGAL::Surface_mesh<CGAL::Point_3<K>>>
{
    using kernel = K;
    using point_type = CGAL::Point_3<K>;
};

template <class Point>
struct mesh_traits<CGAL::Surface_mesh<Point>>
{
    using point_type = Point;
};
}
