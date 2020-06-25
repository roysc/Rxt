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

#define RXT_log_debug(var_) (::Rxt::print("[DEBUG] {}", var_))
#define RXT_log(var_) RXT_log_debug(var_)

#define RXT_show_impl(var_) RXT_log( #var_ " = {}\n", (var_))
#define RXT_show(...) RXT_show_impl(__VA_ARGS__)
// #define RXT_show_n(...) FOR(var_, __VA_ARGS__, RXT_show(var_))

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
