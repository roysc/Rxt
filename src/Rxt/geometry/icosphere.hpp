#pragma once

#include <CGAL/Surface_mesh.h>
#include <CGAL/subdivision_method_3.h>
#include <cmath>

namespace Rxt
{
template <class Point_3>
auto create_icosphere(float r, int n, CGAL::Surface_mesh<Point_3>& ret)
{
    // constexpr float circsphere_radius = 0.951056516295;
    // CGAL::Surface_mesh<Point_3> ret;
    const Point_3 center {CGAL::ORIGIN};
    // ret.clear();

    make_icosahedron(ret, center, r);
    CGAL::Subdivision_method_3::Loop_subdivision(ret, CGAL::parameters::number_of_iterations(n));

    // normalize - fixme: just adjust radius...
    for (auto& p: ret.points()) {
        auto v = (p - center);
        p = center + (v / std::sqrt(v.squared_length()));
    }
    return ret;
}
}
