#pragma once

#include <type_traits>
#include <limits>

namespace Rxt
{
template <class Int, Int mod>
struct ring
{
    static_assert(std::is_signed_v<Int>, "Underlying type must be signed");

    Int value;

    void _wrap()
    {
        value %= mod;
        if (value < 0)
            value += mod;
    }

    ring(Int v) : value(v) { _wrap(); }
    ring& operator=(Int v) { value = v; _wrap(); return *this; }

    ring& operator+=(Int u) { value += u; _wrap(); return *this; }
    ring& operator-=(Int u) { value -= u; _wrap(); return *this; }
    ring& operator*=(Int u) { value*= u; _wrap(); return *this; }

    operator Int() const { return value; }
};

// template <class Int, Int mod>
// friend bool operator==(ring<Int, mod> a, auto const& b)
// {
//     return a.value == b;
// }

// template <class Int, Int mod>
// friend bool operator==(auto const& a, ring<Int, mod> b)
// {
//     return a == b.value;
// }

// template <class Int, Int mod>
// friend bool operator!=(ring<Int, mod> a, auto const& b)
// {
//     return !(a == b);
// }

// template <class Int, Int mod>
// friend bool operator!=(auto const& a, ring<Int, mod> b)
// {
//     return !(a == b);
// }
}
