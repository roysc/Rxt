#include "gl_handy.hpp"

#include "gl_core.hpp"
#include "gl_error.hpp"
#include "_gl_debug.hpp"

#include "Rxt/log.hpp"

#include <cstring>

#ifndef glDebugMessageCallback  // OpenGL < 4.3
#define glDebugMessageCallback(...)                                     \
    RXT_warn("[Emscripten] glDebugMessageCallback not supported")
#define glDebugMessageControl(...)                                      \
    RXT_warn("[Emscripten] glDebugMessageControl not supported")
#endif

namespace Rxt::gl
{
void init_glew()
{
    glewExperimental = GL_TRUE;
    if (auto err = glewInit(); err != GLEW_OK) {
        RXT_error("{}\n", glewGetErrorString(err));
        throw std::runtime_error("glewInit");
    }
}

void _debug_callback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const void*);

void init_debug_output()
{
#ifdef __EMSCRIPTEN__
    RXT_warn("OpenGL debugging not supported\n");
#else
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(_debug_callback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    auto flags = get<GLint>(GL_CONTEXT_FLAGS);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        RXT_info("OpenGL debugging is enabled\n");
    }
#endif
}

program make_program(file_asset_source const& assets, std::string name)
{
    program ret;
    for (auto kind: {GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER}) {
        auto path = assets.find_shader(name, kind);
        if (!exists(path)) {
            if (kind == GL_GEOMETRY_SHADER) continue;
            throw std::invalid_argument(path);
        }
        log("loading: {}\n", path);
        shader sh(kind, Rxt::read_file(path).c_str());
        log_result(sh, GL_COMPILE_STATUS, path.c_str());
        glAttachShader(ret, sh);
    }

    glLinkProgram(ret);
    log_result(ret, GL_LINK_STATUS, "program");
    return ret;
}

void _debug_callback(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar* message,
                     void const*)
{
    if (!debug_context::enable_logging) return;

    const char* kind = "INFO";
    bool severe = true;
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        // Print, log, whatever based on the enums and message
        kind = "ERROR";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        kind = "WARN";
        break;
    default:
        severe = false;
    }
    if (severe)
        RXT_warn("GL {0}: {1}\n", kind, message);
    else
        RXT_info("GL {0}: {1}\n", kind, message);

    if (type == GL_DEBUG_TYPE_ERROR)
        throw message_error(message);
}
}
