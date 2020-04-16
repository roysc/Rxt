#pragma once

#include <Rxt/graphics/gl_handy.hpp>
#include <tuple>

namespace Rxt::shader_programs
{
struct colored_triangle_3D_data;
using colored_triangle_3D = gl::derived_program<colored_triangle_3D_data>;

struct colored_triangle_3D_data
{
    static const char* program_name() { return "colored_triangle_3D"; }

    static constexpr GLenum draw_mode = GL_TRIANGLES;

    using position_vec = glm::vec3;
    using normal_vec = glm::vec3;
    using color_vec = glm::vec3;
    using vertex = std::tuple<position_vec, normal_vec, color_vec>;
    using size_type = std::size_t;

    colored_triangle_3D& prog;
    gl::vao va;

    gl::attribuf<position_vec> position {prog, "vertex_position"};
    gl::attribuf<normal_vec> normal {prog, "vertex_normal"};
    gl::attribuf<color_vec> color {prog, "vertex_color"};

    gl::uniform<glm::mat4> model_matrix {prog, "M"};
    gl::uniform<glm::mat4> view_matrix {prog, "V"};
    gl::uniform<glm::mat4> mvp_matrix {prog, "MVP"};
    gl::uniform<glm::vec3> light_position {prog, "light_position"};

    colored_triangle_3D_data(colored_triangle_3D& p)
        : prog(p)
    {
        position.enable(va);
        normal.enable(va);
        color.enable(va);
    }

    void update()
    {
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);

        gl::buffer_data(position);
        gl::buffer_data(normal);
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
