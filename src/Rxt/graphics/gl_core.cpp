#include "gl_core.hpp"

#include "Rxt/format.hpp"

namespace Rxt::gl
{
attrib_traits::value_type attrib_traits::create(const object<program_traits>& p, const char* n)
{
    value_type id = glGetAttribLocation(p, n);
    if (id == value_type(-1)) {
        print("warning: attrib '{0}' is undefined\n", n);
    }
    return id;
}

uniform_traits::value_type uniform_traits::create(const object<program_traits>& p, const char* n)
{
    value_type id = glGetUniformLocation(p, n);
    if (id == value_type(-1)) {
        print("warning: uniform '{0}' is undefined\n", n);
    }
    return id;
}

void vao_traits::destroy(value_type id)
{
    glDeleteVertexArrays(1, &id);
}

void vbo_traits::destroy(value_type id)
{
    glDeleteBuffers(1, &id);
}
}
