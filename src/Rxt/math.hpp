#pragma once

#include "util.hpp"

#include <stdexcept>
#include <cmath>

namespace Rxt
{
inline namespace constants
{
enum class axis3 : unsigned char
{
    x = 0, y = 1, z = 2
};
const axis3 axis3_values[3] {axis3::x, axis3::y, axis3::z};

constexpr axis3 to_axis3(unsigned char a)
{
    if (a > (unsigned)axis3::z) {
        throw std::invalid_argument("bad axis");
    }
    return static_cast<axis3>(a);
}

template <class Vec3>
constexpr Vec3 zero3 {0, 0, 0};

template <class Vec3>
constexpr Vec3 _basis3[3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
};

template <class Vec3>
constexpr Vec3 basis3(axis3 axis)
{
    return _basis3<Vec3>[(unsigned) axis];
}

template <class Vec3>
constexpr Vec3 basis3(unsigned char axis)
{
    return basis3<Vec3>(to_axis3(axis));
}
}
template <class P, class Q>
bool point_within(P p, Q q, Q r = Q{0})
{
    static_assert(P::length() == Q::length());
    auto [a, b] = box(q, r);
    bool ret = true;
    for (int i = 0; i < q.length(); ++i) {
        // if (i > p.length()) {
        //     ret &&= (q[i] == 0);
        //     continue;
        // }
        ret &= (a[i] <= p[i]);
        ret &= (p[i] < b[i]);
    }
    return ret;
}

// Sample a clifford torus - the simplest and most symmetric flat embedding (in R^4)
// of the cartesian product of two circles
template <class FP, class Sampler>
auto sample_clifford_torus(FP nx, FP ny, const double R, Sampler&& sample_4D)
{
    using std::sin;
    using std::cos;
    FP angle_x = tau * nx, angle_y = tau * ny;
    const auto C = R / tau;

    return sample_4D(C * cos(angle_x), C * sin(angle_x),
                     C * cos(angle_y), C * sin(angle_y));
}
}
