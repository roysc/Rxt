#pragma once

#include "space.hpp"
#include "../grid_context.hpp"

#include <Rxt/graphics/sdl.hpp>
#include <Rxt/graphics/color.hpp>
#include <Rxt/graphics/glm.hpp>
#include <Rxt/runtime.hpp>
#include <Rxt/time.hpp>

#include <entt/entt.hpp>
#include <glm/gtx/transform.hpp>

#include <functional>
#include <optional>
#include <variant>
#include <chrono>
#include <random>

using std::optional;
using std::get_if;

using glm::ivec2;
using glm::vec4;

namespace sdl = Rxt::sdl;
namespace gl = Rxt::gl;
using Rxt::dbg::print;
using Rxt::shader_programs::grid_quad_2D;

using steady_clock = std::chrono::steady_clock;
using time_point = steady_clock::time_point;

constexpr float epsilon = 1e-2;

struct grid_selection
{
    using region = std::tuple<grid_coord, grid_size>;
    optional<region> selection;
    optional<grid_coord> drag_origin;
};

struct mug_position
{
    grid_coord coord;
};

struct mug_memory
{
    int count;
};

struct main_context : public grid_context
                    , public Rxt::simple_runtime
{
    using tick_duration = Rxt::duration_fps<30>;

    sdl::key_dispatcher kd;
    std::thread metronome;
    time_point t_last = steady_clock::now();

    grid_coord cursor_position {0}; // relative to viewport
    grid_quad_2D::data b_quads_sticky {quad_prog};
    grid_quad_2D::data b_entities {quad_prog};

    bool p_edge_scroll = false;

    std::variant<std::monostate, grid_selection> current_mode;

    entt::registry registry;
    std::function<void()> move_entities;

    main_context(grid_size, glm::uvec2);
    ~main_context() override { metronome.join(); }

    template <class M>
    M* get_mode() { return get_if<M>(&current_mode); }

    void step();
    void draw();

    void update_entities(tick_duration);

    void update_cursor();
    void update_selection();

    void handle(SDL_Event);
    void h_mouse_motion(SDL_MouseMotionEvent);
    void h_mouse_down(SDL_MouseButtonEvent);
    void h_mouse_up(SDL_MouseButtonEvent);
    void h_edge_scroll();
};

main_context::main_context(grid_size world_size, glm::uvec2 tile_px)
    : grid_context {"demo: gridworld", world_size, tile_px}
{
    kd.on_press["C-W"]    = [this] { should_quit(true); };
    kd.on_press["C"]      = [this] {
        this->viewport_position = grid_coord {0};
        this->update_viewport();
    };
    kd.on_press["Escape"] = [this] {
        current_mode = {};
        this->update_viewport();
        print("Disabled modes\n");
    };

    kd.on_press["E"] = [this] { p_edge_scroll = !p_edge_scroll; };
    kd.on_press["S"] = [this] { current_mode = grid_selection {}; };

    kd.on_press["I"] = [this] {
        print("pos=({0:+}, {1:+}) ", viewport_position.x, viewport_position.y);
        print("size=({0}, {1}) ", viewport_size.x, viewport_size.y);
        print("cursor=({0:+}, {1:+})\n", cursor_position.x, cursor_position.y);
        if (auto select_mode = get_mode<grid_selection>();
            select_mode && select_mode->selection) {
            auto [a, b] = *select_mode->selection;
            print("selected: pos=({}, {}) size=({}, {})\n", a.x, a.y, b.x, b.y);
        }
    };

    kd.on_scan["Left"]  = [this] { h_move_viewport(-1, 0); };
    kd.on_scan["Right"] = [this] { h_move_viewport(+1, 0); };
    kd.on_scan["Down"]  = [this] { h_move_viewport(0, -1); };
    kd.on_scan["Up"]    = [this] { h_move_viewport(0, +1); };
    kd.on_press["."] = std::bind(&grid_context::h_scale_viewport, this, +1);
    kd.on_press[","] = std::bind(&grid_context::h_scale_viewport, this, -1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto image = create_map(world_size, 42);
    this->update_texture(data(image));

    // initialize cursor
    b_quads_sticky.position.storage = {cursor_position};
    b_quads_sticky.color.storage = {vec4(0, 1, 1, .5)};
    b_quads_sticky.size.storage = {vec2(1)};
    b_quads_sticky.update();

    this->update_model();
    this->update_viewport();

    // glClearColor(0, 0.69, 0, 1);
    auto russet = Rxt::rgb_hex(0x80461B);
    glClearColor(russet.r, russet.g, russet.b, 1);

    metronome = sdl::start_metronome(tick_duration {1}, [this] { return !should_quit(); });

    auto e = registry.create();
    registry.assign<mug_position>(e, grid_coord {0});
    registry.assign<mug_memory>(e, 0);
    // auto e2 = registry.create<mug_position, mug_memory>();

    move_entities = [
        this, e,
        dist1 = std::uniform_int_distribution<int>(0, 1),
        dist4 = std::uniform_int_distribution<int>(0, 4)
    ] () mutable {
        thread_local std::random_device rd;
        int dir = dist4(rd);
        if (dist1(rd))
            return;
        ivec2 movement {(dir & 0b01), (dir & 0b10) >> 1};
        movement = movement * 2 - 1;
        auto& e_pos = registry.get<mug_position>(e);
        e_pos.coord += movement;
        if constexpr (is_torus()) {
            e_pos.coord %= this->world_size;
        }
        // print("moving ({}, {})\n", movement.x, movement.y);
    };
}

void main_context::step()
{
    SDL_Event event;
    SDL_WaitEvent(&event);
    do {
        handle(event);
    } while (SDL_PollEvent(&event));

    kd.scan();

    if (p_edge_scroll) {
        h_edge_scroll();
    }

    {
        auto now = steady_clock::now();
        tick_duration dt = now - t_last;
        if (dt.count() > 1) {
            for (int i = 0; i < dt.count(); ++i) {
                move_entities();
            }
            update_entities(dt);
            t_last = now;
        }
    }

    if (is_dirty()) {
        draw();
        set_dirty(false);
    }
}

void main_context::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    this->b_texs.draw();
    this->b_quads.draw();
    b_entities.draw();

    { // todo: use UBOs and ubo_guard
        gl::uniform<grid_coord> u_vpos {quad_prog, "viewportPosition"};
        set(u_vpos, grid_coord {0});
        this->b_quads_sticky.draw();
        set(u_vpos, viewport_position);
    }

    SDL_GL_SwapWindow(window.get());
}

void main_context::update_entities(tick_duration dt)
{
    b_entities.clear();

    auto view = registry.view<mug_position>();
    for (auto e: view) {
        auto& pos = view.get<mug_position>(e);
        b_entities.push(pos.coord, grid_size(1), Rxt::rgba(0, 1, 0, 1));
    }
    b_entities.update();

    set_dirty();
}

void main_context::update_selection()
{
    // render selected regions
    b_quads.clear();
    if (auto select = get_mode<grid_selection>()) {
        for (auto [a, b]: Rxt::to_range(select->selection)) {
            b_quads.push(a, b, Rxt::rgba(Rxt::colors::hot_pink, 0.5)); //todo
        }
    }
    b_quads.update();

    set_dirty();
}

void main_context::update_cursor()
{
    b_quads_sticky.position.storage[0] = cursor_position;
    b_quads_sticky.size.storage[0] = grid_size(1);

    if (auto select = get_mode<grid_selection>()) {
        // render cursor drag area
        if (auto& origin = select->drag_origin) {
            auto [a, b] = Rxt::ordered(*origin, cursor_position);
            b_quads_sticky.position.storage[0] = a;
            b_quads_sticky.size.storage[0] = b - a + 1;
        }
    }
    b_quads_sticky.update();

    set_dirty();
}

void main_context::h_mouse_motion(SDL_MouseMotionEvent motion)
{
    auto [x, y] = sdl::nds_coords(*window, motion.x, motion.y);
    cursor_position = nds_to_grid(vec2(x, y), vec2(viewport_size / 2u));
    update_cursor();
}

void main_context::h_mouse_down(SDL_MouseButtonEvent button)
{
    switch (button.button) {
    case SDL_BUTTON_LEFT: {
        if (auto select = get_mode<grid_selection>()) {
            select->drag_origin = cursor_position;
            this->update_cursor();
        }
        break;
    }
    }
}

void main_context::h_mouse_up(SDL_MouseButtonEvent button)
{
    switch (button.button) {
    case SDL_BUTTON_LEFT: {
        if (auto select = get_mode<grid_selection>()) {
            if (auto& origin = select->drag_origin) {
                auto [a, b] = Rxt::ordered(*origin, cursor_position);
                select->selection.emplace(a + viewport_position, b - a + 1);
                select->drag_origin = {};
                this->update_cursor();
                this->update_selection();
            }
        }
        break;
    }
    }
}

// edge-of-screen cursor scrolling
void main_context::h_edge_scroll()
{
    // (0,0) is center-screen, so offset it to the corner
    ivec2 offset_pos = cursor_position + ivec2(viewport_size / 2u);
    ivec2 dv {0};
    for (unsigned i = 0; i < dv.length(); ++i) {
        if (offset_pos[i] == 0) {
            dv[i] = -1;
        } else if (offset_pos[i] + 1 == viewport_size[i]) {
            dv[i] = +1;
        }
    }
    if (dv != ivec2 {0}) {
        h_move_viewport(dv.x, dv.y);
    }
}

void main_context::handle(SDL_Event event)
{
    switch (event.type) {
    case SDL_QUIT: { should_quit(true); return; }
    case SDL_KEYDOWN: { kd.press(event.key.keysym); break; }
    case SDL_MOUSEMOTION: { h_mouse_motion(event.motion); break; }
    case SDL_MOUSEBUTTONDOWN: { h_mouse_down(event.button); break; }
    case SDL_MOUSEBUTTONUP: { h_mouse_up(event.button); break; }
    }
}
