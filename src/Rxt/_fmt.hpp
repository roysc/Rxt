#pragma once

#ifndef RXT_DISABLE_FMT
    #define FMT_STRING_ALIAS 1
    #include <fmt/format.h>
    #include <fmt/ostream.h>
#else
    #include <iostream>
#endif

#include <utility>
#include <cstdio>

namespace Rxt
{
#ifndef RXT_DISABLE_FMT
namespace _fmt = ::fmt;
using _fmt::print;

template <class...T>
void print_to(std::FILE* out, T&&...a)
{
    print(out, std::forward<T...>(a)...);
}
#else
namespace _fmt {}

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
}
