#pragma once

#include "_gl_debug.hpp"
#include "Rxt/io.hpp"

#include <cstdlib>
#include <filesystem>
#include <memory>
#include <string>
#include <utility>

#ifndef RXT_GRAPHICS_DATA_DEFAULT
#define RXT_GRAPHICS_DATA_DEFAULT "./data"
#endif

namespace Rxt::gl
{
struct file_loader
{
    std::string data_path_envvar = "RXT_GRAPHICS_DATA";
    std::filesystem::path default_path = RXT_GRAPHICS_DATA_DEFAULT;

    auto data_root() const
    {
        auto ret = default_path;
        if (auto r = std::getenv(data_path_envvar.c_str())) {
            ret = r;
        }
        return ret;
    }

    auto shader_root() const
    {
        return data_root() / "shader";
    }

    // Assumes shaders like: name.vert, name.frag
    gl::program find_program(std::string name) const
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
};

file_loader const& thread_file_loader();
}
