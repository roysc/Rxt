#pragma once

#include "_fmt.hpp"

#include <string>
#include <fstream>
#include <sstream>

namespace Rxt
{
inline std::string read_file(std::string const& p)
{
    std::ifstream in{p};
    in.exceptions(std::ios::failbit | std::ios::badbit);

    std::stringstream buf;
    buf << in.rdbuf();
    return buf.str();
}
}
