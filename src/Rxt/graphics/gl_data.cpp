#include "gl_data.hpp"
#include <Rxt/format.hpp>

namespace Rxt::gl::_det
{
// #ifndef glUniform2ui            // not in WebGL2
// #define glUniform2ui glUniform2i
// #endif

#define _span_ptr(v_)( glm::value_ptr((v_)[0]))

template <>
void data_setter<GLint>::set_value(value_type id, GLint val)
{
    glUniform1i(id, val);
}

template <>
void data_setter<GLint>::set_array(value_type id, span<GLint const> val)
{
    glUniform1iv(id, val.size(), val.data());
}

template <>
void data_setter<GLuint>::set_value(value_type id, GLuint val)
{
    glUniform1ui(id, val);
}

template <>
void data_setter<GLuint>::set_array(value_type id, span<GLuint const> val)
{
    glUniform1uiv(id, val.size(), val.data());
}

using namespace vec;

template <>
void data_setter<fvec2>::set_value(value_type id, fvec2 v)
{
    glUniform2f(id, v.x, v.y);
}

template <>
void data_setter<fvec2>::set_array(value_type id, span<fvec2 const> v)
{
    glUniform2fv(id, v.size(), _span_ptr(v));
}

template <>
void data_setter<ivec2>::set_value(value_type id, ivec2 v)
{
    glUniform2i(id, v.x, v.y);
}

template <>
void data_setter<ivec2>::set_array(value_type id, span<ivec2 const> v)
{
    glUniform2iv(id, v.size(), _span_ptr(v));
}

template <>
void data_setter<uvec2>::set_value(value_type id, uvec2 v)
{
    glUniform2ui(id, v.x, v.y);
}

template <>
void data_setter<uvec2>::set_array(value_type id, span<uvec2 const> v)
{
    glUniform2uiv(id, v.size(), _span_ptr(v));
}

template <>
void data_setter<fvec3>::set_value(value_type id, fvec3 v)
{
    glUniform3f(id, v.x, v.y, v.z);
}

template <>
void data_setter<fvec3>::set_array(value_type id, span<fvec3 const> v)
{
    glUniform3fv(id, v.size(), _span_ptr(v));
}

template <>
void data_setter<fvec4>::set_value(value_type id, fvec4 v)
{
    glUniform4f(id, v.x, v.y, v.z, v.w);
}

template <>
void data_setter<fvec4>::set_array(value_type id, span<fvec4 const> v)
{
    glUniform4fv(id, v.size(), _span_ptr(v));
}

template <>
void data_setter<fmat3>::set_value(value_type id, fmat3 m)
{
    glUniformMatrix3fv(id, 1, GL_FALSE, value_ptr(m));
}

template <>
void data_setter<fmat3>::set_array(value_type id, span<fmat3 const> ms)
{
    glUniformMatrix3fv(id, ms.size(), GL_FALSE, _span_ptr(ms));
}

template <>
void data_setter<fmat4>::set_value(value_type id, fmat4 m)
{
    glUniformMatrix4fv(id, 1, GL_FALSE, value_ptr(m));
}

template <>
void data_setter<fmat4>::set_array(value_type id, span<fmat4 const> ms)
{
    glUniformMatrix4fv(id, ms.size(), GL_FALSE, _span_ptr(ms));
}

// // struct data_setter<bool> : _det::data_adaptor<GL_UNSIGNED_BYTE> // todo: ok?
// {
//     void set_value(value_type id, bool x)
//     {
//         glUniform1i(id, x);
//     }

//     void set_array(value_type id, span<bool const> val)
//     {
//         glUniform1iv(id, val.size(), static_cast<int const*>(val.data()));
//     }
// };
#undef _span_ptr
}
