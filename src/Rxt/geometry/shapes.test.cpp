#include "shapes.hpp"
#include "Rxt/_testing.hpp"

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>

namespace Rxt
{
TEST_CASE("icosahedron-based sphere")
{
    using K = CGAL::Simple_cartesian<double>;
    CGAL::Surface_mesh<K::Point_3> sm[2];

    make_icosphere(sm[0], 1.0, 0);
    make_icosphere(sm[1], 1.0, 1);

    REQUIRE(sm[0].number_of_vertices() == 12);
    REQUIRE(sm[0].number_of_faces() == 20);
    REQUIRE(sm[1].number_of_vertices() == 42);
    REQUIRE(sm[1].number_of_faces() == 80);
}
}
