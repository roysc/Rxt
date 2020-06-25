#include "io.hpp"
#include "_testing.hpp"

namespace Rxt
{
TEST_CASE("print_to")
{
    int x = 4;
    print("{}", x);
    // assert(_out.str() == "4");
}
}
