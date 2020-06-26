#pragma once

#include <cmath>

namespace Rxt
{
template <class Vec3>
auto normalize(Vec3 v)
{
    return v / std::sqrt(v.squared_length());
}

namespace _det
{
constexpr double epsilon = 0.0001;

// Return a, with a[i] replaced with b[i]
template <class T>
constexpr T replacing(T const& a, T const& b, unsigned i)
{
    switch (i) {
    case 0: return T{ b[0], a[1], a[2] };
    case 1: return T{ a[0], b[1], a[2] };
    case 2: return T{ a[0], a[1], b[2] };
    default:
        assert(false);
        return T{};
    }
}
}
}
