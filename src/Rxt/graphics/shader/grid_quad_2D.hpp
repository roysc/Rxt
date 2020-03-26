#pragma once

#include <Rxt/graphics/gl.hpp>

namespace Rxt::shader_programs
{
struct grid_quad_2D_data;
using grid_quad_2D = gl::derived_program<grid_quad_2D_data>;

struct grid_quad_2D_data
{
    static const char* program_name() { return "grid_quad_2D"; }

    using position_vec = glm::ivec2;
    using size_vec = glm::uvec2;
    using color_vec = glm::vec4;
    static constexpr GLenum draw_mode = GL_POINTS;

    grid_quad_2D& prog;
    gl::vao va;
    gl::attribuf<position_vec> position {prog, "gridPosition"};
    gl::attribuf<size_vec> size {prog, "quadSize"};
    gl::attribuf<color_vec> color {prog, "color"};

    grid_quad_2D_data(grid_quad_2D& p) : prog(p)
    {
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);
        gl::attrib_buffer(position);
        gl::attrib_buffer(size);
        gl::attrib_buffer(color);
        glEnableVertexArrayAttrib(va, position);
        glEnableVertexArrayAttrib(va, size);
        glEnableVertexArrayAttrib(va, color);
    }

    void update()
    {
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);
        gl::buffer_data(position);
        gl::buffer_data(size);
        gl::buffer_data(color);
    }

    void clear()
    {
        position.storage.clear();
        size.storage.clear();
        color.storage.clear();
    }

    void draw()
    {
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);
        // gl::bind_ubo_guard _u(ub); // TODO
        glDrawArrays(draw_mode, 0, std::size(position.storage));
    }

    void push(position_vec p, size_vec s, color_vec c)
    {
        position.storage.push_back(p);
        size.storage.push_back(s);
        color.storage.push_back(c);
    }
};
}
