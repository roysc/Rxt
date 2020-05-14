#pragma once

#include "gl_core.hpp"

#include <cstdlib>
#include <filesystem>
#include <string>

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

    // Assumes shaders named like: name.vert, name.frag
    gl::program find_program(std::string name) const;
};
using program_loader = file_loader; // fixme

inline file_loader& default_program_loader()
{
    thread_local file_loader thread_loader;
    return thread_loader;
}
}
