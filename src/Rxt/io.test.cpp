#include "io.hpp"
#include "_testing.hpp"

namespace Rxt
{
TEST_CASE("RXT_show")
{
    int x = 4;

    // RXT_log_out(file);
    RXT_show(x);
    // assert(out == "[DEBUG] x = 4\n");
}
}
