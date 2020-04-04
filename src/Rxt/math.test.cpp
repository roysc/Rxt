#include "_testing.hpp"

#include "math.hpp"

namespace Rxt
{
TEST_CASE("ring")
{
    using R10 = ring<int, 10>;

    R10 a{5};
    REQUIRE(a == 5);

    a = 15;
    REQUIRE(a == 5);

    a += 6;
    REQUIRE(a == 1);

    a -= 3;
    REQUIRE(a == 8);

    a *= 4;
    REQUIRE(a == 2);

    int x = a;
    REQUIRE(x == 2);
}
}
