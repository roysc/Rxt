#pragma once
#include "sdl_gl.hpp"
#include "sdl_prelude.hpp"

namespace Rxt::sdl
{
// Convenient gui class with T = user data
// T: default constructible
struct simple_gui
{
    const unsigned width, height;
    sdl::sdl_context _sdl;
    sdl::window_ptr window;
    sdl::gl_context _gl;

    template <class T>
    simple_gui(const char* name, T size) : simple_gui(name, size[0], size[1]) {}

    simple_gui(const char* name, unsigned w, unsigned h)
        : width(w), height(h)
        , _sdl()
        , window(sdl::make_shared(SDL_CreateWindow(
                                      name,
                                      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                      width, height,
                                      SDL_WINDOW_OPENGL |
                                      SDL_WINDOW_RESIZABLE)))
        , _gl(window)
    { }
};
}
