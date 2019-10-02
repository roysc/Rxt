#pragma once

#include <CGAL/Kernel/global_functions.h>
#include <CGAL/boost/graph/helpers.h>
#include <CGAL/boost/graph/iterator.h>
#include <boost/property_map/property_map.hpp>

namespace Rxt
{
// https://doc.cgal.org/latest/BGL/BGL_polyhedron_3_2normals_8cpp-example.html
template<class G, class NormalMap>
void calculate_face_normals(G const& g, NormalMap nm)
{
    using GT     = boost::graph_traits<G>;
    using Hd     = typename GT::halfedge_descriptor;
    using Normal = typename boost::property_traits<NormalMap>::value_type;

    auto pm = get(CGAL::vertex_point, g);
    for (auto [fb, fe] = faces(g); fb != fe; ++fb) {
        Hd edg = halfedge(*fb, g), edgb = edg;
        auto p0 = pm[target(edg, g)];
        edg = next(edg, g);
        auto p1 = pm[target(edg, g)];
        edg = next(edg, g);
        auto p2 = pm[target(edg, g)];
        edg = next(edg, g);

        // triangle or not?
        if (edg == edgb) {
            nm[*fb] = CGAL::unit_normal(p1, p2, p0);
        } else {
            Normal n(CGAL::NULL_VECTOR);
            do {
                n = n + CGAL::normal(p1, p2, p0);
                p0 = p1;
                p1 = p2;

                edg = next(edg, g);
                p2 = pm[target(edg, g)];
            } while (edg != edgb);

            nm[*fb] = n / CGAL::sqrt(n.squared_length());
        }
    }
}
}
