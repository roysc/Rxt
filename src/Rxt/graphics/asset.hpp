#pragma once

#include "Rxt/io.hpp"
#include "gl_error.hpp"

#include <cstdlib>
#include <string>
#include <filesystem>

#ifndef RXT_ASSET_PATH_DEFAULT
#define RXT_ASSET_PATH_DEFAULT "./data"
#endif

// TODO: asset_source concept?
namespace Rxt::gl
{
constexpr char const* _shader_suffix(GLuint kind)
{
    switch (kind) {
    case GL_VERTEX_SHADER: return ".vert";
    case GL_GEOMETRY_SHADER: return ".geom";
    case GL_FRAGMENT_SHADER: return ".frag";
    default: throw value_error(kind);
    }
}

struct file_asset_source
{
    using path = std::filesystem::path;
    inline static constexpr char const* s_env_var = "RXT_ASSET_PATH";

    path m_root;

    path root() const
    {
        return m_root;
    }

    file_asset_source(path root = {})
    {
        // Environment overrides macro; passed overrides env
        if (!root.empty()) {
            m_root = root;
        } else if (auto r = std::getenv(s_env_var)) {
            m_root = r;
        } else {
            m_root = RXT_ASSET_PATH_DEFAULT;
        }
    }

    path find_shader(std::string name, GLuint kind) const
    {
        return root() / "shader" / (name + _shader_suffix(kind));
    }

    path find_texture(std::string name) const
    {
        return root() / "texture" / (name + ".png");
    }

    static auto& get_default()
    {
        thread_local file_asset_source thread_instance;
        return thread_instance;
    }
};
}
