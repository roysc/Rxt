#pragma once

#include "Rxt/graphics/gl.hpp"
#include "Rxt/graphics/gl_handy.hpp"

namespace Rxt::shader_programs
{
struct colored_triangle_3D
{
    static constexpr const char* program_name() { return "colored_triangle_3D"; }

    using position_vec = glm::vec3;
    using normal_vec = glm::vec3;
    using color_vec = glm::vec3;

    gl::program& prog;
    gl::vao va;

    gl::attribuf<position_vec> position {prog, "position"};
    gl::attribuf<normal_vec> normal {prog, "normal"};
    gl::attribuf<color_vec> color {prog, "color"};

    colored_triangle_3D(gl::program& p)
        : prog(p)
    {
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);

        gl::attrib_buffer(position);
        gl::attrib_buffer(normal);
        gl::attrib_buffer(color);
        glEnableVertexArrayAttrib(va, position);
        glEnableVertexArrayAttrib(va, normal);
        glEnableVertexArrayAttrib(va, color);
    }

    void update()
    {
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);

        gl::buffer_data(position);
        gl::buffer_data(tex_coord);
        gl::buffer_data(elements, GL_ELEMENT_ARRAY_BUFFER);
    }

    void draw()
    {
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);
        glDrawArrays(GL_TRIANGLES, 0, std::size(position.storage));
    }

    void clear()
    {
        position.storage.clear();
        normal.storage.clear();
        color.storage.clear();
    }

    void push(position_vec p, normal_vec n, color_vec c)
    {
        position.storage.push_back(p);
        normal.storage.push_back(n);
        color.storage.push_back(c);
    }
};
}
