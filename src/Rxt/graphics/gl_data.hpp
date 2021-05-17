#pragma once

#include "gl_core.hpp"
#include "gl_guard.hpp"

#include <Rxt/vec.hpp>
#include <Rxt/span.hpp>
#include <Rxt/format.hpp>

namespace Rxt::gl
{
inline const vec::fvec3
    AXIS_X {1, 0, 0},
    AXIS_Y {0, 1, 0},
    AXIS_Z {0, 0, 1};

namespace _det
{
template <class T>
struct data_setter
{
    using value_type = GLuint;
    static void set_value(value_type, T);
    static void set_array(value_type, span<T const>);
};

template <GLenum Enum, GLint D0 = 1, GLint D1 = 1>
struct data_adaptor
{
    using value_type = GLuint;
    static constexpr GLenum data_enum = Enum;
    static constexpr GLint dimensions_0 = D0;
    static constexpr GLint dimensions_1 = D1;
    static constexpr bool is_integral =
        data_enum != GL_HALF_FLOAT &&
        data_enum != GL_FLOAT &&
        data_enum != GL_DOUBLE;
};
}

template <class T>
void set(uniform<T>& u, T const& x)
{
    use_guard _p(u.program());
    _det::data_setter<T>::set_value(u, x);
}

template <class T>
void set_array(uniform<T>& u, span<T> xs)
{
    use_guard _p(u.program());
    _det::data_setter<T>::set_array(u, xs);
}

template <class T>
void set_index(uniform<T>& u, unsigned i, T x)
{
    auto name = format("{}[{}]", u.name(), i);
    set_uniform(u.program(), name.c_str(), x);
}

template <class T>
void set_uniform(program& prog, const char* n, T const& x)
{
    uniform<T> u(prog, n);
    set(u, x);
}

template <class T>
struct data_traits;

template <> struct data_traits<GLint> : _det::data_adaptor<GL_INT> {};
template <> struct data_traits<GLuint> : _det::data_adaptor<GL_UNSIGNED_INT> {};
template <> struct data_traits<vec::fvec2> : _det::data_adaptor<GL_FLOAT, 2> {};
template <> struct data_traits<vec::ivec2> : _det::data_adaptor<GL_INT, 2> {};
template <> struct data_traits<vec::uvec2> : _det::data_adaptor<GL_UNSIGNED_INT, 2> {};
template <> struct data_traits<vec::fvec3> : _det::data_adaptor<GL_FLOAT, 3> {};
template <> struct data_traits<vec::fvec4> : _det::data_adaptor<GL_FLOAT, 4> {};
template <> struct data_traits<vec::fmat3> : _det::data_adaptor<GL_FLOAT, 3, 3> {};
template <> struct data_traits<vec::fmat4> : _det::data_adaptor<GL_FLOAT, 4, 4> {};
// template <> struct data_traits<bool> : _det::data_adaptor<GL_UNSIGNED_BYTE> {};
}
