#pragma once

#include "gl_core.hpp"
#include "gl_data.hpp"
#include "gl_guard.hpp"
#include "gl_loader.hpp"
#include "_gl_debug.hpp"

#include <Rxt/meta.hpp>
#include <vector>
#include <utility>

namespace Rxt::gl
{
void setup_glew();
void setup_debug();

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

    void enable(vao&);
};

template <class T>
void enable_attrib(attrib<T>& attr, vao& va)
{
#ifdef RXT_WEBGL2               // OpenGL < 4.5 compatibility
    glBindVertexArray(va);
    glEnableVertexAttribArray(attr);
    glBindVertexArray(0);
#else
    glEnableVertexArrayAttrib(va, attr);
#endif
}

template <class T>
void attrib_buffer(attribuf<T>& b)
{
    using Traits = gl::data_traits<T>;
    bind_buffer_guard _b{GL_ARRAY_BUFFER, b.vbo};

    if constexpr (Traits::is_integral) {
        glVertexAttribIPointer(b, Traits::dimensions_0, Traits::data_enum, 0, nullptr);
    } else {
        glVertexAttribPointer(b, Traits::dimensions_0, Traits::data_enum, GL_FALSE, 0, nullptr);
    }
}

template <class T>
void buffer_data(buffer<T>& b, GLenum target = GL_ARRAY_BUFFER)
{
    bind_buffer_guard _b{target, b.vbo};
    glBufferData(target, std::size(b.storage) * sizeof(T), std::data(b.storage), GL_STATIC_DRAW);
}

template <class T>
void attribuf<T>::enable(vao& va)
{
    use_guard _p{this->prog};
    bind_vao_guard _a(va);
    attrib_buffer(*this);
    enable_attrib(*this, va);
}

namespace _det
{
template <class D>
using program_uniforms_t = typename D::uniforms;

template <class T>
using uniforms_base =
    std::experimental::detected_or_t<meta::swallow, program_uniforms_t, T>;
}

template <class Buffers>
struct derived_program : public program
{
    using buffers = Buffers;
    using data = buffers; //depr.
    using vertex = typename buffers::vertex;
    using uniforms = _det::uniforms_base<buffers>;

    uniforms u_{*this};

    derived_program(program_loader const& loader = default_program_loader())
        : program{loader.find_program(buffers::program_name())}
    {}

    uniforms* operator->() { return &u_; }
};
}
