#pragma once

#include "mesh_traits.hpp"

#include <CGAL/Origin.h>
#include <CGAL/subdivision_method_3.h>

#include <cmath>

namespace Rxt
{
template <class Mesh, class Point>
auto make_cuboid(Mesh& mesh, Point min, Point max)
{
    // CGAL_precondition(mesh.is_valid());
    auto x0 = min.x(), y0 = min.y(), z0 = min.z();
    auto x1 = max.x(), y1 = max.y(), z1 = max.z();

    return CGAL::make_hexahedron(
        Point{x0, y0, z0},
        Point{x1, y0, z0},
        Point{x1, y1, z0},
        Point{x0, y1, z0},
        Point{x0, y1, z1},
        Point{x0, y0, z1},
        Point{x1, y0, z1},
        Point{x1, y1, z1},
        mesh
    );
}

template <class Mesh>
auto make_icosphere(Mesh& mesh, float r, unsigned n)
{
    // constexpr float circsphere_radius = 0.951056516295; ?
    using Point = typename mesh_traits<Mesh>::point_type;
    const Point center {CGAL::ORIGIN};

    make_icosahedron(mesh, center, r);
    CGAL::Subdivision_method_3::Loop_subdivision(mesh, CGAL::parameters::number_of_iterations(n));

    for (auto& point: mesh.points()) {
        auto v = (point - center);
        point = center + (v / std::sqrt(v.squared_length()));
    }
    return mesh;
}
}
