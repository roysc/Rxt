#pragma once

#include "Rxt/log.hpp"
#include "Rxt/vec.hpp"

#include <SOIL/SOIL.h>

#include <utility>
#include <memory>
#include <optional>
#include <stdexcept>

namespace Rxt
{
inline auto load_image(const char* path)
{
    int width, height, nchannels;
    auto image = SOIL_load_image(path, &width, &height, &nchannels, SOIL_LOAD_AUTO);
    if (image) {
        RXT_info("Loaded image: [w, h, nc]=[{0},{1},{2}]\n", width, height, nchannels);
    } else {
        RXT_warn("Failed to load image ({0}): {1}\n", path, SOIL_last_result());
        throw std::invalid_argument(SOIL_last_result());
    }

    return std::pair(
        std::unique_ptr<unsigned char, void(*)(unsigned char*)>(image, &SOIL_free_image_data),
        vec::uvec2(width, height)
    );
}
}
