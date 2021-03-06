#include "gl_handy.hpp"
#include "_gl_debug.hpp"
#include "gl_core.hpp"

#ifndef glDebugMessageCallback  // OpenGL < 4.3
#define glDebugMessageCallback(...)                                     \
    ::Rxt::dbg::print("[Emscripten] glDebugMessageCallback not supported")
#define glDebugMessageControl(...)                                      \
    ::Rxt::dbg::print("[Emscripten] glDebugMessageControl not supported")
#endif

namespace Rxt::gl
{
void setup_glew()
{
    glewExperimental = GL_TRUE;
    if (auto err = glewInit(); err != GLEW_OK) {
        dbg::print("{}\n", glewGetErrorString(err));
        throw std::runtime_error("glewInit");
    }
}

void _debug_callback(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*, const void*);

void setup_debug()
{
#ifdef __EMSCRIPTEN__
    dbg::print("OpenGL debugging not supported\n");
#else
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(_debug_callback, &g_debug_context);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

    auto flags = get<GLint>(GL_CONTEXT_FLAGS);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        dbg::print("OpenGL debugging is enabled\n");
    }
#endif
}

program make_program(make_program_arg shaders)
{
    program ret;

    for (auto [type, path]: shaders) {
        gl::shader sh(type, Rxt::read_file(path).c_str());
        log_result(sh, GL_COMPILE_STATUS, path.c_str());
        glAttachShader(ret, sh);
    }

    glLinkProgram(ret);
    log_result(ret, GL_LINK_STATUS, "program");
    return ret;
}

void _debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                     const GLchar* message, void const* user_param)
{
    auto& data = *static_cast<debug_context const*>(user_param);
    if (!data.enable_logging) return;

    const char* kind = "INFO";
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        // Print, log, whatever based on the enums and message
        kind = "ERROR";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        kind = "WARN";
        break;
    }
    dbg::print("GL {0}: {1}\n", kind, message);
}
}
