#pragma once

namespace Rxt
{
namespace constants
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
}
