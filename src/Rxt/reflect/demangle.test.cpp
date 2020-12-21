#include "demangle.hpp"
#include "Rxt/_testing.hpp"

namespace Rxt
{
TEST_CASE("demangle")
{
    auto ni = type_name<int>();
    REQUIRE_EQ(ni, "i");
}
}
