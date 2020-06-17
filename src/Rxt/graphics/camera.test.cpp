#include "camera.hpp"
#include "../_testing.hpp"

namespace Rxt
{
TEST_CASE("focus_cam") {
    glm::vec3 p1(1);
    focus_cam cam{p1};
    REQUIRE(cam.position() == p1);
}

TEST_CASE("focus_cam_crt") {
    int data = 0;
    using namespace _det;
    struct camera : focus_cam_crt<camera>
    {
        using focus_cam_crt<camera>::focus_cam_crt;
        int* data;
        void set_position(position_type) { *data = 4; }
    };

    using Pos = camera::position_type;
    Pos p1{0}, p2{0};
    camera c(p1); c.data = &data;
    REQUIRE(c.position() == p1);

    c.position(p2);
    REQUIRE(c.position() == p2);
    REQUIRE(data == 4);
}
}
