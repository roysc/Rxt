#include "math.hpp"
#include "_testing.hpp"

#include "Rxt/vec.hpp"

namespace Rxt
{
TEST_CASE("point_within")
{
    fvec2 p{1,2}, q{0,1}, r{2,3};
    REQUIRE(point_within(p, q, r));
    REQUIRE(!point_within(q, p, r));
}

TEST_CASE("sample_clifford_torus")
{
    // int width = 4, height = 4;
    // int seed;

    // OSN::Noise<4> osn4(seed);
    // auto noise_4D = [&] (auto... args) { return osn4.eval(args...); };
    // auto get_noise = [&] (glm::vec2 c, float r) {
    //     return Rxt::sample_clifford_torus(c.x, c.y, r, noise_4D);
    // };

}
}
