#pragma once

#include "../_debug.hpp"

#include <SOIL/SOIL.h>

#include <utility>
#include <memory>
#include <optional>

namespace Rxt
{
inline auto load_image(const char* path, int& width, int& height)
{
    int nchannels;
    auto image = SOIL_load_image(path, &width, &height, &nchannels, SOIL_LOAD_RGB);
    if (image) {
        dbg::print("Image load: [w, h, nc]=[{0},{1},{2}]\n", width, height, nchannels);
    } else {
        dbg::print("Failed to load image (at {0})\n", path);
    }

    std::unique_ptr<unsigned char, void(*)(unsigned char*)> ret{image, &SOIL_free_image_data};
    return ret;
}
}
