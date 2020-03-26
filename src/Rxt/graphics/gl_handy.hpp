#pragma once

#include "gl_core.hpp"
#include "gl_data.hpp"
#include "gl_guard.hpp"

#include <vector>
#include <utility>

namespace Rxt::gl
{
void setup_glew();
void setup_debug();
// void setup_3d();

using make_program_arg = std::vector<std::pair<GLenum, std::string>>;
program make_program(make_program_arg);

template <class T>
struct buffer
{
    gl::vbo vbo;
    std::vector<T> storage;
};

template <class T>
struct attribuf : public attrib<T>, public buffer<T>
{
    using attrib<T>::attrib;
};

template <class T>
void buffer_data(buffer<T>& b, GLenum target = GL_ARRAY_BUFFER)
{
    bind_buffer_guard _b(target, b.vbo);
    glBufferData(target, std::size(b.storage) * sizeof(T), std::data(b.storage), GL_STATIC_DRAW);
}

template <class T>
void attrib_buffer(attribuf<T>& b)
{
    bind_buffer_guard _b(GL_ARRAY_BUFFER, b.vbo);
    glVertexAttribPointer(b,
                          gl::data_traits<T>::dimensions_0,
                          gl::data_traits<T>::gl_type,
                          GL_FALSE, 0, nullptr); // normalized, stride, array buffer offset
}
}
