#pragma once
#include "gl.hpp"
#include <cassert>

namespace Rxt::gl
{
struct use_guard
{
    use_guard(program& p) { glUseProgram(p); }
    ~use_guard() { glUseProgram(0); }
};

struct bind_texture_guard
{
    GLenum target;
    bind_texture_guard(GLenum tgt, texture& t)
        : target(tgt)
    {
        glBindTexture(tgt, t);
        assert(glIsTexture(t));
    }
    ~bind_texture_guard() { glBindTexture(target, 0); }
};

struct bind_buffer_guard
{
    GLenum target;
    bind_buffer_guard(GLenum tgt, vbo& t)
        : target(tgt)
    {
        glBindBuffer(tgt, t);
        assert(glIsBuffer(t));
    }
    ~bind_buffer_guard() { glBindBuffer(target, 0); }
};

struct bind_vao_guard
{
    bind_vao_guard(vao& t) { glBindVertexArray(t); }
    ~bind_vao_guard() { glBindVertexArray(0); }
};

}
