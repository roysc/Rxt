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
using Str = std::string;
struct asset_loader : file_loader
{
    Str env_var = "RXT_ASSET_PATH";

    asset_loader()
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
    program find_program(Str name) const;

    texture find_texture(Str name) const;

    static asset_loader& default_loader()
    {
        thread_local asset_loader thread_loader;
        return thread_loader;
    }
};
}
