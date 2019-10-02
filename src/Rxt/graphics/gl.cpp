#include "gl.hpp"

namespace Rxt::gl
{
void vao_traits::destroy(value_type id)
{
    glDeleteVertexArrays(1, &id);
}

void vbo_traits::destroy(value_type id)
{
    glDeleteBuffers(1, &id);
}
}
