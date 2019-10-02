#pragma once

#include "Rxt/graphics/gl.hpp"
#include "Rxt/graphics/gl_handy.hpp"

namespace Rxt
{
struct texture_quad
{
    gl::program& prog;
    gl::vao va;
    gl::texture tex;
    gl::attribuf<glm::vec2> position {prog, "position"};
    gl::attribuf<glm::vec2> tex_coord {prog, "texCoord"};
    gl::buffer<GLuint> elements;

    texture_quad(gl::program& p)
        : prog(p)
    {
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);

        gl::attrib_buffer(position);
        gl::attrib_buffer(tex_coord);
        glEnableVertexArrayAttrib(va, position);
        glEnableVertexArrayAttrib(va, tex_coord);

        gl::set_uniform(prog, "texUnit", 0);
    }

    void update()
    {
        gl::use_guard _p(prog);
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
        gl::use_guard _p(prog);
        gl::bind_vao_guard _a(va);
        gl::bind_texture_guard _t(GL_TEXTURE_2D, tex);
        gl::bind_buffer_guard _b(GL_ELEMENT_ARRAY_BUFFER, elements.vbo);
        glDrawElements(GL_TRIANGLES, std::size(elements.storage), GL_UNSIGNED_INT, 0);
    }
};
}
