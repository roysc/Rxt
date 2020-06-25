#pragma once

#include "Rxt/graphics/gl.hpp"
#include "Rxt/math.hpp"

#include <glm/glm.hpp>
#include <boost/multi_array.hpp>
#include <OpenSimplexNoise.hh>

auto create_map(glm::uvec2 size, int seed)
{
    using glm::vec2;

    int width = size.x, height = size.y;
    OSN::Noise<4> osn4(seed);
    auto noise_4D = [&] (auto... args) { return osn4.eval(args...); };
    auto get_noise = [&] (vec2 c, float r) {
        return Rxt::sample_clifford_torus(c.x, c.y, r, noise_4D);
    };

    using Vec4u8 = glm::tvec4<GLubyte, glm::packed_highp>;
    boost::multi_array<Vec4u8, 2> image(boost::extents[width][height]);

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            vec2 c = vec2(x, y) / vec2(size);
            float sample = 0;
            for (int i = 0; i < 8; ++i) {
                sample += (1.f / (i+1)) * get_noise(c, 1 << i);
            }
            image[x][y] = Vec4u8(sample * 0xFF / 2);
        }
    }
    return image;
}
