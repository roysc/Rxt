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
#include <stdexcept>

namespace Rxt
{
#ifndef RXT_DISABLE_FMT
using fmt::print;

template <class...T>
void print_to(std::FILE* out, T&&...a)
{
    print(out, std::forward<T...>(a)...);
}
#else

template <class... T>
void print_to(std::ostream& out, const char* m, T&&... args)
{
    out << m;
    (out << ... << args);
}

template <class... T>
void print(const char* m, T&&... args)
{
    print_to(std::cout, std::forward<T...>(args)...);
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
