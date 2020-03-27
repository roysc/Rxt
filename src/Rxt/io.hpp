#pragma once

#define FMT_STRING_ALIAS 1
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <string>
#include <fstream>
#include <sstream>

namespace Rxt
{
using fmt::print;

inline std::string read_file(std::string const& p)
{
    std::ifstream in(p);
    in.exceptions(std::ios::failbit | std::ios::badbit);

    std::stringstream buf;
    buf << in.rdbuf();
    return buf.str();
}
}
