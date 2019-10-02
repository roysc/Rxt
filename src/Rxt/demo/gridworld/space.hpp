#pragma once
#include "Rxt/graphics/gl.hpp"

#include <glm/glm.hpp>
#include <boost/multi_array.hpp>
#include <OpenSimplexNoise.hh>

#include <cmath>

using vec2 = glm::vec2;
using uvec2 = glm::uvec2;

using Vec4u8 = glm::tvec4<GLubyte, glm::packed_highp>;

template <class T>
using grid_map = boost::multi_array<T, 2>;

auto create_checkerboard(uvec2 size)
{
    int width = size.x, height = size.y;
    std::vector<Vec4u8> image;
    for (int w = 0; w < width; ++w) {
        for (int h = 0; h < height; ++h) {
            image.emplace_back(static_cast<GLubyte>((w + h) % 2 == 0) * 0xFF);
        }
    }
    return image;
}

// Clifford torus: the simplest and most symmetric flat embedding (in R^4)
// of the cartesian product of two circles
template <class FP, class F4D>
auto sample_clifford_torus(FP nx, FP ny, F4D&& sample_4D, const double R)
{
    using std::sin;
    using std::cos;
    const double TAU = 2 * M_PI;
    FP angle_x = TAU * nx, angle_y = TAU * ny;
    const auto C = R / TAU;

    return sample_4D(C * cos(angle_x), C * sin(angle_x),
                     C * cos(angle_y), C * sin(angle_y));
}

auto create_map(uvec2 size, int seed)
{
    int width = size.x, height = size.y;

    OSN::Noise<4> osn4(42);
    auto noise_4D = [&] (auto... args) { return osn4.eval(args...); };
    auto get_noise = [&] (vec2 c, float r) {
        return sample_clifford_torus(c.x, c.y, noise_4D, r);
    };

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
    // image[0][0] = Vec4u8(0xFF, 0, 0, 0xFF); // red origin
    return image;
}
