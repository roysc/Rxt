#pragma once
#include "gl.hpp"
#include "gl_guard.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Rxt::gl
{
inline const glm::vec3 AXIS_X {1, 0, 0};
inline const glm::vec3 AXIS_Y {0, 1, 0};
inline const glm::vec3 AXIS_Z {0, 0, 1};

struct _default_data_traits
{
    using value_type = GLuint;
    static constexpr GLenum gl_type = GL_FLOAT;
};

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
struct data_traits<glm::vec2> : _default_data_traits
{
    static constexpr unsigned dimensions_0 = 2;
    static constexpr unsigned dimensions_1 = 1;

    static void set(value_type id, glm::vec2 const& v)
    {
        glUniform2f(id, v.x, v.y);
    }
};

template <>
struct data_traits<glm::ivec2> : _default_data_traits
{
    static constexpr unsigned dimensions_0 = 2;
    static constexpr unsigned dimensions_1 = 1;

    static void set(value_type id, glm::ivec2 const& v)
    {
        glUniform2i(id, v.x, v.y);
    }
};

template <>
struct data_traits<glm::uvec2> : _default_data_traits
{
    static constexpr unsigned dimensions_0 = 2;
    static constexpr unsigned dimensions_1 = 1;

    static void set(value_type id, glm::uvec2 const& v)
    {
        glUniform2ui(id, v.x, v.y);
    }
};

template <>
struct data_traits<glm::vec3> : _default_data_traits
{
    static constexpr unsigned dimensions_0 = 3;
    static constexpr unsigned dimensions_1 = 1;

    static void set(value_type id, glm::vec3 const& v)
    {
        glUniform3f(id, v.x, v.y, v.z);
    }
};

template <>
struct data_traits<glm::vec4> : _default_data_traits
{
    static constexpr unsigned dimensions_0 = 4;
    static constexpr unsigned dimensions_1 = 1;

    static void set(value_type id, glm::vec4 const& v)
    {
        glUniform4f(id, v.x, v.y, v.z, v.w);
    }
};

template <>
struct data_traits<glm::mat3> : _default_data_traits
{
    static constexpr unsigned dimensions_0 = 3;
    static constexpr unsigned dimensions_1 = 3;

    static void set(value_type id, glm::mat3 const& m)
    {
        glUniformMatrix3fv(id, 1, GL_FALSE, value_ptr(m));
    }
};

template <>
struct data_traits<glm::mat4> : _default_data_traits
{
    static constexpr unsigned dimensions_0 = 4;
    static constexpr unsigned dimensions_1 = 4;

    static void set(value_type id, glm::mat4 const& m)
    {
        glUniformMatrix4fv(id, 1, GL_FALSE, value_ptr(m));
    }
};

template <>
struct data_traits<GLint> : _default_data_traits
{
    static constexpr GLenum gl_type = GL_INT;
    static constexpr unsigned dimensions_0 = 1;
    static constexpr unsigned dimensions_1 = 1;

    static void set(value_type id, GLint x)
    {
        glUniform1i(id, x);
    }
};

template <>
struct data_traits<bool> : _default_data_traits
{
    static constexpr GLenum gl_type = GL_INT;
    static constexpr unsigned dimensions_0 = 1;
    static constexpr unsigned dimensions_1 = 1;

    static void set(value_type id, bool x)
    {
        glUniform1i(id, x);
    }
};
}
