#include "log.hpp"
#include "_testing.hpp"

namespace Rxt
{
std::stringstream _out;
// auto& _out =;
TEST_CASE("RXT_show")
{
    int x = 4;
    // RXT_log_out(file);
    RXT_show(x);
    // assert(_out == "[DEBUG] x = 4\n");
}
}
