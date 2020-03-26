#include "_gl_debug.hpp"
#include "gl_loader.hpp"
#include "Rxt/io.hpp"

#include <utility>

namespace Rxt::gl
{
gl::program file_loader::find_program(std::string name) const
{
    gl::program ret;
    auto paths = {
        std::pair(GL_VERTEX_SHADER, (shader_root()/(name + ".vert"))),
        {GL_GEOMETRY_SHADER, shader_root()/(name + ".geom")},
        {GL_FRAGMENT_SHADER, shader_root()/(name + ".frag")},
    };

    for (auto [kind, path]: paths) {
        if (!exists(path)) {
            if (kind == GL_GEOMETRY_SHADER)
                continue;
            throw std::invalid_argument(path);
        }

        gl::shader sh(kind, Rxt::read_file(path).c_str());
        _log_result(sh, GL_COMPILE_STATUS, path.c_str());
        glAttachShader(ret, sh);
    }

    glLinkProgram(ret);
    _log_result(ret, GL_LINK_STATUS, "program");
    return ret;
}
}
