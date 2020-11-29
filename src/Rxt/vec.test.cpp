#include "vec.hpp"
#include "Rxt/_testing.hpp"

#include <iostream>
#include <unordered_set>

namespace Rxt
{
TEST_CASE("fvec2")
{
    vec::fvec2 v(0), u(1);
    REQUIRE_EQ(v + u, u);
}

TEST_CASE("std::hash definitions")
{
    std::unordered_set<vec::fvec2> s;
    s.emplace(0);
}

// TEST_CASE("stream operator")
// {
//     fvec2 v(1);
//     std::cout << "vec = " << v << "\n";
// }
}
