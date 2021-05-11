#pragma once

#ifndef RXT_DISABLE_FMT
    #include "_fmt.hpp"
#else
    #include <iostream>
#endif

#include <string>
#include <fstream>
#include <sstream>

namespace Rxt
{
#ifdef RXT_DISABLE_FMT
template <class... T>
void print_to(std::ostream& out, const char* m, T&&... args)
{
    out << m;
    (out << ... << args << " ");
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
