#pragma once

#include <GL/glew.h>            // must be included before gl.h
#include <cassert>

#include "../_debug.hpp"

#define _debug_assert assert

namespace Rxt::gl
{
// Ripped: https://stackoverflow.com/a/17161788/1325447
template <class Traits>
struct object
{
    typename Traits::value_type _value;

    template <class... Args>
    object(Args&&... args) : _value(Traits::create(std::forward<Args>(args)...)) {}
    ~object() { Traits::destroy(_value); }

    object(const object&) = delete;
    object& operator=(const object&) = delete;
    object(object&&) = default;
    object& operator=(object&&) = default;

    operator typename Traits::value_type() const { return _value; }
};

// Program
struct program_traits
{
    using value_type = GLuint;

    static constexpr auto getparam = &glGetProgramiv;
    static constexpr auto getinfolog = &glGetProgramInfoLog;

    static value_type create()
    {
        value_type id = glCreateProgram();
        _debug_assert(glIsProgram(id));
        return id;
    }

    static void destroy(value_type id)
    {
        glDeleteProgram(id);
    }
};

// Shader
struct shader_traits
{
    using value_type = GLuint;

    static constexpr auto getparam = &glGetShaderiv;
    static constexpr auto getinfolog = &glGetShaderInfoLog;

    static value_type create(GLenum type, const char* source)
    {
        // create and compile
        value_type id = glCreateShader(type);
        _debug_assert(glIsShader(id));
        glShaderSource(id, 1, &source, nullptr);
        glCompileShader(id);
        return id;
    }

    static void destroy(value_type id)
    {
        glDeleteShader(id);
    }
};

// Vertex array object
struct vao_traits
{
    using value_type = GLuint;
    static value_type create()
    {
        value_type id;
        glGenVertexArrays(1, &id);
        glBindVertexArray(id);
        _debug_assert(glIsVertexArray(id));
        return id;
    }
    static void destroy(value_type);
};

// Vertex buffer object
struct vbo_traits
{
    using value_type = GLuint;
    static value_type create()
    {
        value_type id;
        glGenBuffers(1, &id);
        // _debug_assert(glIsBuffer(id));
        return id;
    }
    static void destroy(value_type);
};

struct texture_traits
{
    using value_type = GLuint;
    static value_type create()
    {
        value_type id;
        glGenTextures(1, &id);
        // _debug_assert(glIsTexture(id));
        return id;
    }
    static void destroy(value_type id)
    {
        glDeleteTextures(1, &id);
    }
};

using program = object<program_traits>;
using shader = object<shader_traits>;
using vao = object<vao_traits>;
using vbo = object<vbo_traits>;
using texture = object<texture_traits>;

template <class Traits, class T>
struct property
{
    typename Traits::value_type _value;
    program& prog;

    template <class... Args>
    property(program& p, Args&&... args)
        : _value(Traits::create(p, std::forward<Args>(args)...))
        , prog(p) {}

    operator typename Traits::value_type() const { return _value; }
};

struct attrib_traits
{
    using value_type = GLuint;
    static value_type create(const object<program_traits>& p, const char* n)
    {
        value_type id = glGetAttribLocation(p, n);
        // _debug_assert(id != -1);
        if (id == value_type(-1)) {
            dbg::print("warning: attrib '{0}' is undefined\n", n);
        }
        return id;
    }
};

// Uniform
struct uniform_traits
{
    using value_type = GLuint;
    static value_type create(const object<program_traits>& p, const char* n)
    {
        value_type id = glGetUniformLocation(p, n);
        // _debug_assert(id != -1);
        if (id == value_type(-1)) {
            dbg::print("warning: uniform '{0}' is undefined\n", n);
        }
        return id;
    }
};

template <class T>
using attrib = property<attrib_traits, T>;
template <class T>
using uniform = property<uniform_traits, T>;

// Reading GL parameters
// Traits for scalar "simple state variables"
template <class T>
struct scalar_traits;

template <>
struct scalar_traits<GLboolean>
{
    static constexpr auto get = &glGetBooleanv;
};

template <>
struct scalar_traits<GLint>
{
    static constexpr auto get = &glGetIntegerv;
};

template <class T>
T get(GLenum pname)
{
    T ret {};
    scalar_traits<T>::get(pname, &ret);
    return ret;
}
}
