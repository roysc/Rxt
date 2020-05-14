#include "time.hpp"
#include "_testing.hpp"

namespace Rxt
{
TEST_CASE("duration_fps")
{
    auto d = duration_fps<30>(1);
    REQUIRE(d.count() == 1);
}
}
