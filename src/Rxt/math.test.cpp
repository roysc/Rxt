#include "math.hpp"
#include "_testing.hpp"

#include <glm/glm.hpp>

namespace Rxt
{
TEST_CASE("point_within")
{
    glm::vec2 p{1,2}, q{0,1}, r{2,3};
    REQUIRE(point_within(p, q, r));
    REQUIRE(!point_within(q, p, r));
}
}
