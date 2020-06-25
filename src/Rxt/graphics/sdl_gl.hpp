#pragma once

#include "sdl_core.hpp"
#include "gl_core.hpp"
#include "gl_handy.hpp"
#include "_sdl_debug.hpp"

#include <Rxt/macro.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <utility>

namespace Rxt::sdl
{
struct sdl_context
{
    sdl_context()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            log_and_fail("SDL_Init");
        }

#ifdef RXT_WEBGL2
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
#endif
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    }

    RXT_def_ctors_move_only(sdl_context);
};

struct gl_context
{
    window_ptr window;
    SDL_GLContext _gl;

    gl_context(window_ptr w)
        : window(w)
        , _gl(SDL_GL_CreateContext(window.get()))
    {
        if (!_gl) {
            log_and_fail("SDL_GL_CreateContext");
        }
        if (auto r = SDL_GL_MakeCurrent(window.get(), _gl) < 0) {
            log_and_fail("SDL_GL_MakeCurrent");
        }
#ifndef __EMSCRIPTEN__
        SDL_GL_SetSwapInterval(1);
#endif

        gl::setup_glew();
        gl::setup_debug();
    }

    ~gl_context()
    {
        SDL_GL_DeleteContext(_gl);
    }

    RXT_def_ctors_move_only(gl_context);
};

// Interface for loop context
template <class T>
bool is_stopped(T const& c) { return !c.is_stopped(); }

template <class T>
void advance(T& c, SDL_Event e) { c.advance(e); }

// Convenience function
template <class C>
void em_advance(void* c)
{
#ifdef __EMSCRIPTEN__
    // Emscripten wants main loop set before swap interval
    thread_local bool _initialized = false;
    if (!_initialized) {
        SDL_GL_SetSwapInterval(1);
        _initialized = true;
    }
#endif

    C& context = *static_cast<C*>(c);
    SDL_Event event;
    // SDL_PumpEvents();

#ifdef __EMSCRIPTEN__
    if (is_stopped(context)) {
        emscripten_cancel_main_loop(); // asynchronous apparently, so also return early
        return;
    }

    if (!SDL_PollEvent(&event))
        return;
#else
    SDL_WaitEvent(&event);
#endif

    advance(context, event);
}

// Emscripten-compatible abstraction over main loop
template <class C>
struct emscripten_looper
{
    using context_ptr =
#ifndef __EMSCRIPTEN__
        std::unique_ptr<C>
#else
        C*                     // don't delete context with emscripten
#endif
        ;
    // using step_function = bool(*)(C&);
    using update_function = void(*)(void*);

    context_ptr context;
    update_function step_context;

    emscripten_looper(C* c, update_function f)
        : context(c), step_context(f) { }

    auto operator()()
    {
#ifndef __EMSCRIPTEN__
        while (!is_stopped(*context)) {
            step_context(&*context);
        }
#else
        emscripten_set_main_loop_arg(step_context, static_cast<void*>(context), 0, true);
#endif
    }
};

template <class T, class F>
auto em_looper(T* c, F&& f)
{
    return emscripten_looper<T> {c, f};
}

auto make_looper = [](auto* c, auto&& f) { return em_looper(c, f); };

// SDL coords -> normalized device space
inline
std::pair<float, float> nds_coords(SDL_Window& window, int x, int y)
{
    int w, h;
    SDL_GetWindowSize(&window, &w, &h);
    float nds_x = float(2 * x) / w - 1, nds_y = 1 - float(2 * y) / h;
    return {nds_x, nds_y};
}

inline
auto nds_coords(SDL_Window& window)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return nds_coords(window, x, y);
}
}
