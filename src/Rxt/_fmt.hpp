#pragma once

#define FMT_STRING_ALIAS 1
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <utility>
#include <cstdio>

namespace Rxt
{
// namespace _fmt = ::fmt;
using ::fmt::print;
using ::fmt::format;

template <class...T>
void print_to(std::FILE* out, T&&...a)
{
    print(out, std::forward<T...>(a)...);
}
}
