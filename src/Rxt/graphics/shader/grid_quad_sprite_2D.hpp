#pragma once

#include "Rxt/graphics/gl_handy.hpp"

#include <tuple>

namespace Rxt::shader_programs
{
struct grid_quad_sprite_2D_data;
using grid_quad_sprite_2D = gl::derived_program<grid_quad_sprite_2D_data>;

struct grid_quad_sprite_2D_data
{
    using program_type = grid_quad_sprite_2D;
    static const char* program_name() { return "grid_quad_sprite_2D"; }

    static constexpr GLenum draw_mode = GL_TRIANGLES;

    using position_attr = glm::ivec2;
    using tex_coord_attr = glm::ivec2;
    using size_attr = glm::uvec2;
    using sprite_index_attr = GLint;
    using vertex = std::tuple<position_attr, tex_coord_attr, size_attr, sprite_index_attr>;

    using size_type = std::size_t;

    program_type& prog;
    gl::texture tex;
    gl::vao va;
    gl::attribuf<position_attr> position {program(), "vertexPosition"};
    gl::attribuf<tex_coord_attr> tex_coord {program(), "texCoord"};
    gl::attribuf<sprite_index_attr> sprite_index{program(), "spriteIndex"};
    gl::buffer<GLuint> elements;
    GLint texture_units[1] = {0}; // arbitrary ids

    struct uniforms
    {
        program_type& prog;
        gl::uniform<position_attr> viewport_position{prog, "viewportPosition"};
        gl::uniform<size_attr> viewport_size{prog, "viewportSize"};
        gl::uniform<GLint> tex_unit{prog, "texUnit"};
        gl::uniform<position_attr> offsets{prog, "offsets"};
    };

    program_type& program() { return prog; }
    gl::texture& texture() { return tex; }

    grid_quad_sprite_2D_data(program_type& p)
        : prog(p)
    {
        gl::use_guard _p(program());
        gl::bind_vao_guard _a(va);

        position.enable(va);
        tex_coord.enable(va);
        sprite_index.enable(va);
        gl::set(program()->tex_unit, texture_units[0]);
    }

    void update()
    {
        gl::use_guard _p(program());
        gl::bind_vao_guard _a(va);
        gl::buffer_data(position);
        gl::buffer_data(tex_coord);
        gl::buffer_data(sprite_index);
        gl::buffer_data(elements, GL_ELEMENT_ARRAY_BUFFER);
    }

    void clear()
    {
        position.storage.clear();
        tex_coord.storage.clear();
        sprite_index.storage.clear();
        elements.storage.clear();
    }

    void draw()
    {
        gl::use_guard _p(program());
        gl::bind_vao_guard _a(va);
        glActiveTexture(GL_TEXTURE0 + texture_units[0]);
        gl::bind_texture_guard _t(GL_TEXTURE_2D, texture());
        gl::bind_buffer_guard _b(GL_ELEMENT_ARRAY_BUFFER, elements.vbo);
        glDrawElements(draw_mode, std::size(elements.storage), GL_UNSIGNED_INT, 0);
    }

    // Push a full quad
    void push(position_attr p, tex_coord_attr c, size_attr s, sprite_index_attr ix)
    {
        auto offset = static_cast<GLuint>(position.storage.size());
        position_attr q = p + position_attr(s);
        tex_coord_attr d = c + tex_coord_attr(s);
        position_attr pos_corners[] = {position_attr(p.x, q.y), q, {q.x, p.y}, p};
        tex_coord_attr tex_corners[] = {tex_coord_attr(c.x, d.y), d, {d.x, c.y}, c};
        for (int i = 0; i < 4; ++i) {
            position.storage.push_back(pos_corners[i]);
            tex_coord.storage.push_back(tex_corners[i]);
            sprite_index.storage.push_back(ix);
        }
        for (GLuint index: {0, 1, 2, 2, 3, 0}) {
            elements.storage.push_back(offset + index);
        }
    }

    // Set quad at a specific index
    void set(size_type index, vertex v)
    {
        index *= 4;
        auto [p, c, s, si] = v;
        position_attr q = p + position_attr(s);
        tex_coord_attr d = c + tex_coord_attr(s);
        position_attr pos_corners[] = {position_attr(p.x, q.y), q, {q.x, p.y}, p};
        tex_coord_attr tex_corners[] = {tex_coord_attr(c.x, d.y), d, {d.x, c.y}, c};
        for (int i = 0; i < 4; ++i, ++index) {
            position.storage.at(index) = pos_corners[i];
            tex_coord.storage.at(index) = tex_corners[i];
            sprite_index.storage.at(index) = si;
        }
    }
};
}
