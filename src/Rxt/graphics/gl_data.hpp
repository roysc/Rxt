#pragma once

#include "gl.hpp"
#include "gl_guard.hpp"
#include "glm.hpp"

namespace Rxt::gl
{
// #ifndef glUniform2ui            // not in WebGL2
// #define glUniform2ui glUniform2i
// #endif

inline const glm::vec3 AXIS_X {1, 0, 0};
inline const glm::vec3 AXIS_Y {0, 1, 0};
inline const glm::vec3 AXIS_Z {0, 0, 1};

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
}

template <class T>
struct data_traits;

template <class T>
void set(uniform<T>& u, T const& x)
{
    gl::use_guard _p(u.prog);
    data_traits<T>::set(u, x);
}

template <class T>
void set_uniform(program& prog, const char* n, T const& x)
{
    gl::uniform<T> u(prog, n);
    set(u, x);
}

template <>
struct data_traits<glm::vec2> : _det::data_adaptor<GL_FLOAT, 2>
{
    static void set(value_type id, glm::vec2 const& v)
    {
        glUniform2f(id, v.x, v.y);
    }
};

template <>
struct data_traits<glm::ivec2> : _det::data_adaptor<GL_INT, 2>
{
    static void set(value_type id, glm::ivec2 const& v)
    {
        glUniform2i(id, v.x, v.y);
    }
};

template <>
struct data_traits<glm::uvec2> : _det::data_adaptor<GL_UNSIGNED_INT, 2>
{
    static void set(value_type id, glm::uvec2 const& v)
    {
        glUniform2ui(id, v.x, v.y);
    }
};

template <>
struct data_traits<glm::vec3> : _det::data_adaptor<GL_FLOAT, 3>
{
    static void set(value_type id, glm::vec3 const& v)
    {
        glUniform3f(id, v.x, v.y, v.z);
    }
};

template <>
struct data_traits<glm::vec4> : _det::data_adaptor<GL_FLOAT, 4>
{
    static void set(value_type id, glm::vec4 const& v)
    {
        glUniform4f(id, v.x, v.y, v.z, v.w);
    }
};

template <>
struct data_traits<glm::mat3> : _det::data_adaptor<GL_FLOAT, 3, 3>
{
    static void set(value_type id, glm::mat3 const& m)
    {
        glUniformMatrix3fv(id, 1, GL_FALSE, value_ptr(m));
    }
};

template <>
struct data_traits<glm::mat4> : _det::data_adaptor<GL_FLOAT, 4, 4>
{
    static void set(value_type id, glm::mat4 const& m)
    {
        glUniformMatrix4fv(id, 1, GL_FALSE, value_ptr(m));
    }
};

template <>
struct data_traits<GLint> : _det::data_adaptor<GL_INT>
{
    static void set(value_type id, GLint x)
    {
        glUniform1i(id, x);
    }
};

template <>
struct data_traits<bool> : _det::data_adaptor<GL_UNSIGNED_BYTE> // todo: ok?
{
    static void set(value_type id, bool x)
    {
        glUniform1i(id, x);
    }
};
}
