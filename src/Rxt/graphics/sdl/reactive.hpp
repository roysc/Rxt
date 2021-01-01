#pragma once

#include "Rxt/reactive.hpp"
#include "Rxt/graphics/sdl_gui.hpp"

// Convenient CRTP child with hook-based handlers
namespace Rxt::sdl
{
struct input_hooks : input_handler_crt<input_hooks>
{
    Rxt::hooks<> on_quit;
    Rxt::hooks<SDL_Keysym> on_key_down;
    Rxt::hooks<SDL_MouseButtonEvent> on_mouse_down, on_mouse_up;
    Rxt::hooks<SDL_MouseMotionEvent> on_mouse_motion;
    Rxt::hooks<SDL_MouseWheelEvent> on_mouse_wheel;
};
}
