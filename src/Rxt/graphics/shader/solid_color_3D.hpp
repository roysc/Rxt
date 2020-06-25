#pragma once

#include <Rxt/graphics/gl_handy.hpp>

namespace Rxt::shader_programs
{
template <GLenum> struct solid_color_3D_data;
template <GLenum Mode>
using solid_color_3D = gl::derived_program<solid_color_3D_data<Mode>>;

template <GLenum Mode>
struct solid_color_3D_data
{
    using program_type = solid_color_3D<Mode>;
    static const char* program_name() { return "solid_color_3D"; }

    static constexpr GLenum draw_mode = Mode;

    using position_vec = glm::vec3;
    using color_vec = glm::vec3;
    using vertex = std::tuple<position_vec, color_vec>;

    program_type& prog;
    gl::vao va;
    gl::attribuf<position_vec> position {prog, "vertexPosition"};
    gl::attribuf<color_vec> color {prog, "vertexColor"};

    struct uniforms
    {
        program_type& prog;
        gl::uniform<glm::mat4> mvp_matrix {prog, "MVP"};
    };

    program_type& program() { return prog; }

    solid_color_3D_data(program_type& p)
        : prog(p)
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
    }

    void draw()
    {
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);
        glDrawArrays(draw_mode, 0, std::size(position.storage));
    }

    void clear()
    {
        position.storage.clear();
        color.storage.clear();
    }

    void push(position_vec p, color_vec c)
    {
        position.storage.push_back(p);
        color.storage.push_back(c);
    }
};
}
