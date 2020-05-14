#pragma once

#include "grid_space.hpp"
#include "grid_context.hpp"

#include "Rxt/graphics/sdl.hpp"
#include "Rxt/graphics/color.hpp"
#include "Rxt/util.hpp"

#include <optional>
#include <variant>
#include <chrono>

namespace sdl = Rxt::sdl;

using tick_duration = std::chrono::duration<float, std::ratio<1, 30>>;

struct grid_selection
{
    using region = std::tuple<grid_coord, grid_size>;
    std::optional<region> selection;
    std::optional<grid_coord> drag_origin;
};

struct grid_world : public grid_context
                  , public Rxt::simple_runtime
{
    sdl::key_dispatcher keys;
#ifndef __EMSCRIPTEN__
    sdl::metronome metronome{tick_duration{1}, [this] { return !should_quit(); }};
#endif
    grid_quad_2D::data b_quads_sticky {quad_prog};
    bool p_edge_scroll = true;

    grid_coord cursor_position {0}; // relative to viewport
    const Rxt::rgba cursor_color{Rxt::colors::cyan, .5};

    std::variant<std::monostate, grid_selection> current_mode;

    grid_world(grid_size, glm::uvec2);

    template <class M>
    M* get_mode() { return std::get_if<M>(&current_mode); }

    void step(SDL_Event);
    void draw();

    void update_cursor();
    void update_selection();

    void handle(SDL_Event);
    void h_mouse_motion(SDL_MouseMotionEvent);
    void h_mouse_down(SDL_MouseButtonEvent);
    void h_mouse_up(SDL_MouseButtonEvent);
    void h_edge_scroll();
};
