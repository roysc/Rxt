#pragma once

#ifndef RXT_DISABLE_FMT
    #include "_fmt.hpp"
#else
    #include <iostream>
#endif

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

// template <class... T>
// void println(const char* m, T&&... args)
// {
//     print(m, std::forward<T...>(args)...);
//     print("\n");
// }
}
