#include "image.hpp"

#include "Rxt/log.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace Rxt
{
load_image_result load_image(const char* path)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, nchannels;
    auto image = stbi_load(path, &width, &height, &nchannels, 0);
    if (image) {
        RXT_info("Loaded image: [w, h, nc]=[{0},{1},{2}]\n", width, height, nchannels);
    } else {
        auto reason = stbi_failure_reason();
        RXT_warn("Failed to load image ({0}): {1}\n", path, reason);
        throw std::runtime_error(reason);
    }

    return std::pair(
        std::unique_ptr<unsigned char, void(*)(void*)>(image, &stbi_image_free),
        vec::uvec2(width, height)
    );
}
}
