#include "io.hpp"
#include "_testing.hpp"

namespace Rxt
{
TEST_CASE("debug macro")
{
    int x = 4;
    // int y = 5;

    // _set_debug_ostream(out);
    Rxt_DEBUG(x);
    // assert(out == "[DEBUG] x = 4\n");
}
}
