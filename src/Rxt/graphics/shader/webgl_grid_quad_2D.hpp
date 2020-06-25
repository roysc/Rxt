#pragma once

#include <Rxt/graphics/gl_handy.hpp>

#include <tuple>

namespace Rxt::shader_programs::webgl
{
struct grid_quad_2D_data;
using grid_quad_2D = gl::derived_program<grid_quad_2D_data>;

// WebGL-compatible program with same interface as grid_quad_2D w/ geometry shader
struct grid_quad_2D_data
{
    using program_type = grid_quad_2D;
    static const char* program_name() { return "webgl_grid_quad_2D"; }

    static constexpr GLenum draw_mode = GL_TRIANGLES;

    using position_vec = glm::ivec2;
    using size_vec = glm::uvec2;
    using color_vec = glm::vec4;
    using vertex = std::tuple<position_vec, size_vec, color_vec>;
    using size_type = std::size_t;

    program_type& prog;
    gl::vao va;
    gl::attribuf<position_vec> position {prog, "quadPosition"};
    gl::attribuf<color_vec> color {prog, "color"};
    gl::buffer<GLuint> elements;

    struct uniforms
    {
        program_type& prog;
        gl::uniform<glm::ivec2> viewport_position{prog, "viewportPosition"};
        gl::uniform<glm::uvec2> viewport_size{prog, "viewportSize"};
    };

    program_type& program() { return prog; }

    grid_quad_2D_data(program_type& p) : prog(p)
    {
        position.enable(va);
        color.enable(va);
    }

    void update()
    {
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);
        gl::buffer_data(position);
        gl::buffer_data(color);
        gl::buffer_data(elements, GL_ELEMENT_ARRAY_BUFFER);
    }

    void clear()
    {
        position.storage.clear();
        color.storage.clear();
    }

    void draw()
    {
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);
        gl::bind_buffer_guard _b(GL_ELEMENT_ARRAY_BUFFER, elements.vbo);
        glDrawElements(draw_mode, std::size(elements.storage), GL_UNSIGNED_INT, 0);
    }

    void push(position_vec p, size_vec s, color_vec c)
    {
        // Build a quad
        auto offset = static_cast<GLuint>(position.storage.size());
        position_vec q = p + glm::ivec2(s);
        for (auto pos: {position_vec{p.x, q.y}, q, {q.x, p.y}, p}) {
            position.storage.push_back(pos);
            color.storage.push_back(c);
        }
        for (GLuint index: {0, 1, 2, 2, 3, 0}) {
            elements.storage.push_back(offset + index);
        }
    }

    void set(size_type index, vertex v)
    {
        index *= 4;
        auto [p, s, c] = v;
        position_vec q = p + position_vec{s};
        for (auto pos: {position_vec{p.x, q.y}, q, {q.x, p.y}, p}) {
            position.storage.at(index) = pos;
            color.storage.at(index++) = c;
        }
    }
};
}
