#include "vec.hpp"
#include "Rxt/_testing.hpp"

#include <iostream>
#include <unordered_set>

namespace Rxt::vec
{
TEST_CASE("basic")
{
    {
        fvec2 v(0), u(1);
        REQUIRE_EQ(v + u, u);
    }
    {
        fvec3 v(1,2,3);
        REQUIRE_EQ(squared_length(v), 14);
        REQUIRE_EQ(product(v), 6);
        ivec3 vi(1,2,3);
        REQUIRE_EQ(squared_length(vi), 14);
        REQUIRE_EQ(product(vi), 6);
    }
}

TEST_CASE("std::hash definitions")
{
    std::unordered_set<fvec2> s;
    s.emplace(0);
}

TEST_CASE("unravel_index")
{
    uvec1 shape1(3);
    REQUIRE_EQ(unravel_index(2, shape1), uvec1(2));

    uvec2 shape2(3);
    REQUIRE_EQ(unravel_index(4, shape2), uvec2(1, 1));

    shape2 = uvec2(4,1);
    REQUIRE_EQ(unravel_index(3, shape2), uvec2(3, 0));
    shape2 = uvec2(1,4);
    REQUIRE_EQ(unravel_index(3, shape2), uvec2(0, 3));

    uvec3 shape3(3);
    REQUIRE_EQ(unravel_index(10, shape3), uvec3(1,0,1));

    // print("i={} shape={} unravelled={}\n",unravel_index(3, shape));
}
}
