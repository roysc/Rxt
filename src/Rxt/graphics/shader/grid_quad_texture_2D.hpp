#pragma once

#include "Rxt/io.hpp"
#include "Rxt/graphics/gl_handy.hpp"

#include <tuple>

namespace Rxt::shader_programs
{
struct grid_quad_texture_2D_data;
using grid_quad_texture_2D = gl::derived_program<grid_quad_texture_2D_data>;

struct grid_quad_texture_2D_data
{
    using program_type = grid_quad_texture_2D;
    static const char* program_name() { return "grid_quad_texture_2D"; }

    static constexpr GLenum draw_mode = GL_TRIANGLES;

    using position_vec = glm::ivec2;
    using tex_coord_vec = glm::ivec2;
    using size_vec = glm::uvec2;
    using vertex = std::tuple<position_vec, tex_coord_vec, size_vec>;
    using size_type = std::size_t;

    program_type& prog;
    gl::vao va;
    gl::texture tex;
    gl::attribuf<position_vec> position {program(), "quadPosition"};
    gl::attribuf<tex_coord_vec> tex_coord {program(), "texCoord"};
    gl::buffer<GLuint> elements;

    struct uniforms
    {
        program_type& prog;
        gl::uniform<glm::ivec2> viewport_position{prog, "viewportPosition"};
        gl::uniform<glm::uvec2> viewport_size{prog, "viewportSize"};
        // gl::uniform<gl::sampler2D> tex_unit{prog, 0};
    };

    program_type& program() { return prog; }

    grid_quad_texture_2D_data(program_type& p)
        : prog(p)
    {
        gl::use_guard _p(program());
        gl::bind_vao_guard _a(va);

        position.enable(va);
        tex_coord.enable(va);

        gl::set_uniform(program(), "texUnit", 0);
    }

    void update()
    {
        gl::use_guard _p(program());
        gl::bind_vao_guard _a(va);

        position.storage = {{-1, 1}, {1, 1}, {1, -1}, {-1, -1}};
        tex_coord.storage = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
        elements.storage = {0, 1, 2, 2, 3, 0};
        gl::buffer_data(position);
        gl::buffer_data(tex_coord);
        gl::buffer_data(elements, GL_ELEMENT_ARRAY_BUFFER);
    }

    void draw()
    {
        gl::use_guard _p(program());
        gl::bind_vao_guard _a(va);
        gl::bind_texture_guard _t(GL_TEXTURE_2D, tex);
        gl::bind_buffer_guard _b(GL_ELEMENT_ARRAY_BUFFER, elements.vbo);
        glDrawElements(draw_mode, std::size(elements.storage), GL_UNSIGNED_INT, 0);
    }

    // Push a full quad
    void push(position_vec p, tex_coord_vec c, size_vec s)
    {
        auto offset = static_cast<GLuint>(position.storage.size());
        position_vec q = p + position_vec(s);
        tex_coord_vec d = c + tex_coord_vec(s);
        position_vec pos_corners[] = {position_vec(p.x, q.y), q, {q.x, p.y}, p};
        tex_coord_vec tex_corners[] = {tex_coord_vec(c.x, d.y), d, {d.x, c.y}, c};
        for (int i = 0; i < 4; ++i) {
            position.storage.push_back(pos_corners[i]);
            tex_coord.storage.push_back(tex_corners[i]);
        }
        for (GLuint index: {0, 1, 2, 2, 3, 0}) {
            elements.storage.push_back(offset + index);
        }

        for (auto i: elements.storage) {
            _fmt::print("vertex: {} {}\n", position.storage[i], tex_coord.storage[i]);
        }
    }

    // Set quad at a specific index
    void set(size_type index, vertex v)
    {
        index *= 4;
        auto [p, c, s] = v;
        position_vec q = p + position_vec(s);
        tex_coord_vec d = c + tex_coord_vec(s);
        position_vec pos_corners[] = {position_vec(p.x, q.y), q, {q.x, p.y}, p};
        tex_coord_vec tex_corners[] = {tex_coord_vec(c.x, d.y), d, {d.x, c.y}, c};
        for (int i = 0; i < 4; ++i) {
            position.storage.at(index) = pos_corners[i];
            tex_coord.storage.at(index++) = tex_corners[i];
        }
    }
};
}