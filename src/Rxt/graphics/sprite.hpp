#pragma once

#include <Rxt/graphics/gl.hpp>
#include <Rxt/graphics/image.hpp>
#include <Rxt/vec.hpp>
#include <Rxt/io.hpp>

#include <stdexcept>
#include <string>

namespace Rxt
{
using vec::uvec2;
using vec::ivec2;

// Specification of a sprite sheet
struct sprite_sheet
{
    // gl::texture texture;
    const uvec2 tile_size{16};
    uvec2 frame_size;
    unsigned frame_count;

    ivec2 sprite_position(ivec2 pos) const
    {
        ivec2 texpos{pos.x, frame_size.y - 1 - pos.y};
        return ivec2(tile_size) * (texpos);
    }
};

// struct sprite
// {
//     uvec2 m_sheet_coord;
//     unsigned m_state;
// };

// Infers sheet size given tile size,
// assuming square frame & horizontal frame chain
sprite_sheet infer_sprite_sheet(const uvec2 img_size, const uvec2 tile_size)
{
    if (!all(equal(img_size % tile_size, uvec2(0))))
        throw std::invalid_argument(Rxt::format("incompatible image size ({})", img_size));
    auto frame_side = img_size.y / tile_size.y;
    if (img_size.x % (frame_side * tile_size.x) != 0)
        throw std::invalid_argument(Rxt::format("partial sheet frame ({})", img_size));
    auto frame_count = img_size.x / (frame_side * tile_size.x);
    return {tile_size, uvec2(frame_side), frame_count};
}

// Load the sheet to texture by full path, inferring size and returning spec
sprite_sheet load_sprite_sheet(std::string path, Rxt::gl::texture& tex)
{
    auto [img_data, img_size] = Rxt::load_image(path.c_str());

    gl::bind_texture_guard _t(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        img_size.x, img_size.y,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        img_data.get()
    );

    return infer_sprite_sheet(img_size, uvec2(16));
}

}
