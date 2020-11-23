#pragma once

#include "vec.hpp"
#include <array>

namespace Rxt
{
using rgb = fvec3;
using rgba = fvec4;

inline rgb rgb_hex(unsigned v)
{
    rgb ret;
    for (int i = 0; i < 3; ++i) {
        ret[2-i] = ((v >> (8*i)) & 0xFF);
    }
    return ret / 256.f;
}

inline rgba rgba_hex(unsigned v, float a)
{
    return {rgb_hex(v), a};
}

inline rgba to_rgba(rgb c)
{
    return rgba{c, 1};
}

inline rgba to_rgba(rgb c, float a)
{
    return rgba{c, a};
}

namespace colors
{
inline const rgb red   {1,0,0};
inline const rgb green {0,1,0};
inline const rgb blue  {0,0,1};

inline const rgb cyan   {0,1,1};
inline const rgb violet {1,0,1};
inline const rgb yellow {1,1,0};

inline const rgb black {0};
inline const rgb white {1};
inline const rgb gray  {.5};

inline const rgb brick = rgb_hex(0x963838);
inline const rgb sand = rgb_hex(0xCABB90);
inline const rgb water = rgb_hex(0xB3D1E5);
inline const rgb hot_pink = rgb_hex(0xFF69B4);
inline const rgb russet = rgb_hex(0x80461B);
inline const rgb soil = rgb_hex(0xA0522D);
inline const rgb sky_blue = rgb_hex(0x87CEEB);

// inline const rgba blank {0};
}

// inline rgb default_palette(unsigned i)
// {
//     std::array palette = {
//         colors::black,
//         colors::white,
//         colors::red,
//         colors::green,
//         colors::blue
//     };
//     return palette.at(i);
// }
}
