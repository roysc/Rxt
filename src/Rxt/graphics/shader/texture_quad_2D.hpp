#pragma once

#include "Rxt/graphics/gl_handy.hpp"

#include <tuple>

namespace Rxt::shader_programs
{
struct texture_quad_2D_data;
using texture_quad_2D = gl::derived_program<texture_quad_2D_data>;

struct texture_quad_2D_data
{
    using program_type = texture_quad_2D;
    static const char* program_name() { return "texture_quad_2D"; }

    static constexpr GLenum draw_mode = GL_TRIANGLES;
    // static constexpr auto draw_function = &glDrawElements;

    using position_vec = glm::vec2;
    using tex_coord_vec = glm::vec2;
    using vertex = std::tuple<position_vec, tex_coord_vec>;

    program_type& prog;
    gl::vao va;
    gl::texture tex;
    gl::attribuf<position_vec> position {program(), "position"};
    gl::attribuf<tex_coord_vec> tex_coord {program(), "texCoord"};
    gl::buffer<GLuint> elements;

    struct uniforms
    {
        program_type& prog;
        gl::uniform<glm::mat4> view_matrix{prog, "viewMatrix"}; // todo ubo
    };

    program_type& program() { return prog; }

    texture_quad_2D_data(program_type& p)
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
};
}
