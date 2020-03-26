#pragma once

#include "gl_core.hpp"
#include "gl_handy.hpp"

namespace Rxt
{
template <GLenum mode>
struct solid_color_3D
{
    static const char* program_name() { return "solid_color_3D"; }

    using position_vec = glm::vec3;
    using color_vec = glm::vec3;
    static constexpr GLenum draw_mode = mode;

    static_assert(draw_mode == GL_POINTS ||
                  draw_mode == GL_LINES);

    gl::program& prog;
    gl::vao va;

    gl::attribuf<position_vec> position {prog, "vertexPosition"};
    gl::attribuf<color_vec> color {prog, "vertexColor"};

    gl::uniform<glm::mat4> mvp_matrix {prog, "MVP"};

    solid_color_3D(gl::program& p)
        : prog(p)
    {
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);

        gl::attrib_buffer(position);
        gl::attrib_buffer(color);
        glEnableVertexArrayAttrib(va, position);
        glEnableVertexArrayAttrib(va, color);
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
