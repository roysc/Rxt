#include "camera.hpp"
#include "../_testing.hpp"

namespace Rxt
{
TEST_CASE("focus_cam") {
    int data = 0;
    struct cam : reactive_focus_cam<cam>
    {
        using reactive_focus_cam<cam>::reactive_focus_cam;
        int* data;
        void on_update() { *data = 4; }
    };

    using Pos = cam::position_type;
    Pos p1{0}, p2{0};
    cam c(p1); c.data = &data;
    REQUIRE(c.position() == p1);

    c.position(p2);
    REQUIRE(c.position() == p2);
    REQUIRE(data == 4);
}
}
