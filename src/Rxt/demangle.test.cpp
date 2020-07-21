#include "demangle.hpp"
#include "_testing.hpp"

namespace Rxt
{
TEST_CASE("demangle")
{
    auto ni = type_name<int>();
    REQUIRE_EQUAL(ni, "i");
}
}
