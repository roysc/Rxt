#pragma once

#include "gl_core.hpp"
#include "Rxt/loader.hpp"

#include <cstdlib>
#include <string>

#ifndef RXT_ASSET_PATH_DEFAULT
#define RXT_ASSET_PATH_DEFAULT "./data"
#endif

namespace Rxt::gl
{
struct program_loader : file_loader
{
    std::string env_var = "RXT_ASSET_PATH";

    program_loader()
        : file_loader{RXT_ASSET_PATH_DEFAULT}
    {}

    path data_root() const
    {
        auto ret = root();
        if (auto r = std::getenv(env_var.c_str())) {
            ret = r;
        }
        return ret;
    }

    auto shader_root() const
    {
        return root() / "shader";
    }

    // Assumes shaders named like: name.vert, name.frag
    gl::program find_program(std::string name) const;
};

inline program_loader& default_program_loader()
{
    thread_local program_loader thread_loader;
    return thread_loader;
}
}
