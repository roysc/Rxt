#include "Rxt/_testing.hpp"
#include "color.hpp"
#include "Rxt/vec.hpp"

namespace Rxt
{
TEST_CASE("rgb_hex")
{
    auto black = rgb_hex(0x000000);
    auto white = rgb_hex(0xFFFFFF);
    auto red = rgb_hex(0xFF0000);
    REQUIRE_MESSAGE(black == colors::black, black);
    REQUIRE_MESSAGE(white == colors::white, white);
    REQUIRE_MESSAGE(red == colors::red, red);
}

TEST_CASE("to_rgba")
{
    REQUIRE_EQ(to_rgba(rgb{1,0,0}, 1), rgba{1,0,0,1});
}
}
