#include "Rxt/_testing.hpp"
#include "vec_io.hpp"

#include <iostream>

namespace Rxt
{
TEST_CASE("stream operator")
{
    fvec2 v(1);
    std::cout << "vec = " << v << "\n";
}
}
