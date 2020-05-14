#pragma once

#ifndef RXT_DISABLE_FMT
    #define FMT_STRING_ALIAS 1
    #include <fmt/format.h>
    #include <fmt/ostream.h>
#else
    #include <iostream>
#endif

#include <string>
#include <fstream>
#include <sstream>

#define Rxt_DEBUG_impl(var_) (::Rxt::print("[DEBUG] " #var_ " = {}\n", (var_)))
// #define Rxt_DEBUG_rec(var_, ...) (Rxt_DEBUG_impl(var_), Rxt_DEBUG_rec(__VA_ARGS__))

#define Rxt_DEBUG(...) Rxt_DEBUG_impl(__VA_ARGS__)

namespace Rxt
{
#ifndef RXT_DISABLE_FMT
using fmt::print;
#else
template <class... T>
void print(const char* m, T&&... args)
{
    std::cout << m;
    (std::cout << ... << args);
}
#endif

inline std::string read_file(std::string const& p)
{
    std::ifstream in{p};
    in.exceptions(std::ios::failbit | std::ios::badbit);

    std::stringstream buf;
    buf << in.rdbuf();
    return buf.str();
}
}
