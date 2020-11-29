#pragma once

#include "sdl_gl.hpp"
#include "sdl_core.hpp"

namespace Rxt::sdl
{
struct simple_gui
{
    const unsigned width, height;
    sdl::sdl_context _sdl;
    sdl::window_ptr _window;
    sdl::gl_context _gl;

    template <class T>
    simple_gui(const char* name, T size) : simple_gui(name, size[0], size[1]) {}

    simple_gui(const char* name, unsigned w, unsigned h)
        : width(w), height(h)
        , _sdl()
        , _window(sdl::make_shared(SDL_CreateWindow(
                                      name,
                                      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                      width, height,
                                      SDL_WINDOW_OPENGL |
                                      SDL_WINDOW_RESIZABLE)))
        , _gl(_window)
    { }

    virtual ~simple_gui() {}

    auto& window() { return *_window; }
};

template <class T>
struct gui_input_handler
{
    void handle_input(SDL_Event event)
    {
        T& derived = static_cast<T&>(*this);

        switch (event.type) {
        case SDL_QUIT: { derived.on_quit(); break; }
        case SDL_KEYDOWN: { derived.on_key_down(event.key.keysym); break; }
        case SDL_MOUSEBUTTONDOWN: { derived.on_mouse_down(event.button); break; }
        case SDL_MOUSEBUTTONUP: { derived.on_mouse_up(event.button); break; }
        case SDL_MOUSEMOTION: { derived.on_mouse_motion(event.motion); break; }
        case SDL_MOUSEWHEEL: { derived.on_mouse_wheel(event.wheel); break; }
        }
    }
};

template <class T>
struct gui_input_stubs : public gui_input_handler<T>
{
    void on_quit() {}
    void on_key_down(SDL_Keysym) {}
    void on_mouse_down(SDL_MouseButtonEvent) {}
    void on_mouse_up(SDL_MouseButtonEvent) {}
    void on_mouse_motion(SDL_MouseMotionEvent) {}
    void on_mouse_wheel(SDL_MouseWheelEvent) {}
};
}
