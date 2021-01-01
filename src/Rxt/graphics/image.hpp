#pragma once

#include "Rxt/vec.hpp"

#include <utility>
#include <memory>

namespace Rxt
{
using load_image_result = std::pair<std::unique_ptr<unsigned char, void(*)(void*)>, vec::uvec2>;
load_image_result load_image(const char*);

// SOIL_load_OGL_texture(path, 0, ret, SOIL_FLAG_INVERT_Y);

// inline
// auto create_texture(const unsigned char* data, vec::uvec2 dims)
// {
//     gl::texture ret;
//     SOIL_create_OGL_texture(data, dims.x, dims.y, 0, ret, SOIL_FLAG_INVERT_Y);
//     return ret;
// }
}
