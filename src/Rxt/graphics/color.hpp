#pragma once

#include <glm/glm.hpp>

namespace Rxt
{
using rgb = glm::vec3;
using rgba = glm::vec4;

inline rgb rgb_hex(unsigned v)
{
    glm::vec3 ret;
    for (int i = 0; i < 3; ++i) {
        ret[2-i] = ((v >> (8*i)) & 0xFF);
    }
    return ret / 256.f;
}

inline rgba rgb_hex(unsigned v, float a)
{
    return {rgb_hex(v), a};
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

// inline const rgba blank {0};
}

using palette = std::vector<rgb>;
inline palette default_palette()
{
    return palette {
        colors::black,
        colors::white,
        colors::red,
        colors::green,
        colors::blue
    };
}
}
