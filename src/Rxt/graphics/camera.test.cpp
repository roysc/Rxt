#include "camera.hpp"
#include "../_testing.hpp"

namespace Rxt
{
TEST_CASE("focus_cam") {
    glm::vec3 p1(1);
    focus_cam cam{p1};
    REQUIRE(cam.position() == p1);

    // cam.orbit();
}

TEST_CASE("extending focus_cam") {
    int data = 0;
    using namespace _det;
    struct camera : focus_cam
    {
        using focus_cam::focus_cam;
        using focus_cam::position;
        int* data;
        void position(position_type) override { (*data)++; }
    };

    using Pos = camera::position_type;
    Pos p1{0}, p2{0};
    camera c(p1); c.data = &data;
    REQUIRE(c.position() == p1);

    c.position(p2);
    REQUIRE(c.position() == p2);
    REQUIRE(data == 1);
    c.forward(1);
    REQUIRE(data == 2);
}
}
