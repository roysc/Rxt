#pragma once

#include "gl_core.hpp"
#include "gl_guard.hpp"
#include "Rxt/vec.hpp"
#include "Rxt/span.hpp"

namespace Rxt::gl
{
inline const vec::fvec3
    AXIS_X {1, 0, 0},
    AXIS_Y {0, 1, 0},
    AXIS_Z {0, 0, 1};

namespace _det
{
template <class T>
struct data_traits;
}

using _det::data_traits;

template <class T>
void set(uniform<T>& u, T const& x)
{
    gl::use_guard _p(u.program());
    data_traits<T>::set(u, x);
}

template <class T>
void set(uniform<T>& u, span<T const> xs)
{
    gl::use_guard _p(u.program());
    data_traits<T>::set_array(u, xs);
}

template <class T>
void set_at(uniform<T>& u, T const& x, unsigned i)
{
    auto name = format("{}[{}]", u.name(), i);
    gl::uniform<T> ui(u.program(), name.c_str());
    set(u, x);
}

template <class T>
void set_uniform(program& prog, const char* n, T const& x)
{
    gl::uniform<T> u(prog, n);
    set(u, x);
}

namespace _det
{
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

using namespace vec;

template <>
struct data_traits<GLint> : _det::data_adaptor<GL_INT>
{
    static void set(value_type id, GLint val);
    static void set_array(value_type id, span<GLint const> val);
};

template <>
struct data_traits<GLuint> : _det::data_adaptor<GL_UNSIGNED_INT>
{
    static void set(value_type id, GLint val);
    static void set_array(value_type id, span<GLuint const> val);
};

template <>
struct data_traits<fvec2> : _det::data_adaptor<GL_FLOAT, 2>
{
    static void set(value_type id, fvec2 const& v);
    static void set_array(value_type id, span<fvec2 const> v);
};

template <>
struct data_traits<ivec2> : _det::data_adaptor<GL_INT, 2>
{
    static void set(value_type id, ivec2 const& v);
    static void set_array(value_type id, span<ivec2 const> v);
};

template <>
struct data_traits<uvec2> : _det::data_adaptor<GL_UNSIGNED_INT, 2>
{
    static void set(value_type id, uvec2 const& v);
    static void set_array(value_type id, span<uvec2 const> v);
};

template <>
struct data_traits<fvec3> : _det::data_adaptor<GL_FLOAT, 3>
{
    static void set(value_type id, fvec3 const& v);
    static void set_array(value_type id, span<fvec3 const> v);
};

template <>
struct data_traits<fvec4> : _det::data_adaptor<GL_FLOAT, 4>
{
    static void set(value_type id, fvec4 const& v);
    static void set_array(value_type id, span<fvec4 const> v);
};

template <>
struct data_traits<fmat3> : _det::data_adaptor<GL_FLOAT, 3, 3>
{
    static void set(value_type id, fmat3 const& m);
    static void set_array(value_type id, span<fmat3 const> ms);
};

template <>
struct data_traits<fmat4> : _det::data_adaptor<GL_FLOAT, 4, 4>
{
    static void set(value_type id, fmat4 const& m);
    static void set_array(value_type id, span<fmat4 const> ms);
};

// template <>
// struct data_traits<bool> : _det::data_adaptor<GL_UNSIGNED_BYTE> // todo: ok?
// {
//     static void set(value_type id, bool x)
//     {
//         glUniform1i(id, x);
//     }

//     static void set_array(value_type id, span<bool const> val)
//     {
//         glUniform1iv(id, val.size(), static_cast<int const*>(val.data()));
//     }
// };

}
}
