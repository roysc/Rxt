#include "demangle.hpp"
#include "Rxt/_testing.hpp"

#include <string>

namespace Rxt
{
std::string int_name = "int";

TEST_CASE("demangle")
{
    REQUIRE(demangle(typeid(int).name()) == int_name);
}
TEST_CASE("demangle_type")
{
    REQUIRE(demangle_type<int>() == int_name);
}
}
