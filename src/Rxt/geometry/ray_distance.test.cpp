#include "ray_distance.hpp"
#include "Rxt/_testing.hpp"

#include <CGAL/Simple_cartesian.h>

namespace Rxt
{
TEST_CASE("ray_distance")
{
    // todo
    using K = CGAL::Simple_cartesian<float>;
    using P = K::Point_3;
    using Dist = ray_distance<P, K, ([](auto& p) {return p;})>;
    Dist dist;
}
}