#pragma once
#include "../_debug.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <utility>
#include <memory>
#include <optional>

namespace Rxt
{
auto load_image(std::string path, int& width, int& height)
{
    int nchannels;
    auto image = stbi_load(path.c_str(), &width, &height, &nchannels, 0);
    if (image) {
        dbg::print("stbi load: [w, h, nc]=[{0},{1},{2}]\n", width, height, nchannels);
    } else {
        dbg::print("Failed to load image (at {0})\n", path.c_str());
    }

    std::unique_ptr<unsigned char, void(*)(void*)> ret(image, &stbi_image_free);
    return ret;
}
}
