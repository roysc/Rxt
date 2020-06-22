#include "grid_world.hpp"

#include <glm/gtx/transform.hpp>

#include <exception>
#include <functional>

using Rxt::dbg::print;

extern "C" void step_state(void* c)
{
    sdl::em_advance<grid_world>(c);
}

int main()
{
    glm::uvec2 world_size {32};
    glm::uvec2 tile_size_px {16};

    try {
        auto context = new grid_world(world_size, tile_size_px);
        auto loop = sdl::make_looper(context, step_state);
        loop();

    } catch (std::exception& e) {
        print("error: {}\n", e.what());
    }

    return 0;
}

grid_world::grid_world(grid_size world_size, glm::uvec2 tile_px)
    : grid_context{"demo: grid_world", world_size, tile_px}
{
    keys.on_press["C-W"]    = [this] { should_quit(true); };
    keys.on_press["C"]      = [this] {
        this->viewport_position = grid_coord {0};
        this->update_viewport();
    };
    keys.on_press["Escape"] = [this] {
        current_mode = {};
        this->update_viewport();
        print("Disabled modes\n");
    };

    keys.on_press["E"] = [this] { p_edge_scroll = !p_edge_scroll; };
    keys.on_press["S"] = [this] { current_mode = grid_selection {}; };

    keys.on_press["I"] = [this] {
        print("pos=({0:+}, {1:+}) ", viewport_position.x, viewport_position.y);
        print("size=({0}, {1}) ", viewport_size.x, viewport_size.y);
        print("cursor=({0:+}, {1:+})\n", cursor_position.x, cursor_position.y);
        if (auto select_mode = get_mode<grid_selection>();
            select_mode && select_mode->selection) {
            auto [a, b] = *select_mode->selection;
            print("selected: pos=({}, {}) size=({}, {})\n", a.x, a.y, b.x, b.y);
        }
    };

    keys.on_scan["Left"]  = [this] { h_move_viewport(-1, 0); };
    keys.on_scan["Right"] = [this] { h_move_viewport(+1, 0); };
    keys.on_scan["Down"]  = [this] { h_move_viewport(0, -1); };
    keys.on_scan["Up"]    = [this] { h_move_viewport(0, +1); };
    keys.on_press["."] = std::bind(&grid_context::h_scale_viewport, this, +1);
    keys.on_press[","] = std::bind(&grid_context::h_scale_viewport, this, -1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    auto image = create_map(world_size, 42);
    this->update_texture(data(image));

    this->update_model();
    this->update_viewport();
    // update_cursor();

    // glClearColor(0, 0.69, 0, 1);
    auto russet = Rxt::rgb_hex(0x80461B);
    glClearColor(russet.r, russet.g, russet.b, 1);
}

void grid_world::advance(SDL_Event event)
{
    do {
        handle(event);
    } while (SDL_PollEvent(&event));

    keys.scan();

    if (p_edge_scroll)
        h_edge_scroll();

    if (is_dirty()) {
        draw();
        set_dirty(false);
    }
}

void grid_world::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    this->b_texs.draw();
    this->b_quads.draw();

    {
        auto& vp = quad_prog.u_.viewport_position;
        set(vp, grid_coord{0});
        this->b_quads_sticky.draw();
        set(vp, viewport_position);
    }
    SDL_GL_SwapWindow(window.get());
}

void grid_world::update_selection()
{
    // render selected regions
    b_quads.clear();
    if (auto select = get_mode<grid_selection>()) {
        for (auto [a, b]: Rxt::to_range(select->selection)) {
            b_quads.push(a, b, Rxt::rgba{Rxt::colors::hot_pink, 0.5});
        }
    }
    b_quads.update();

    set_dirty();
}

void grid_world::update_cursor()
{
    grid_coord pos = cursor_position;
    grid_size size{1};

    if (auto select = get_mode<grid_selection>()) {
        // render cursor drag area
        if (auto& origin = select->drag_origin) {
            auto [a, b] = Rxt::box(*origin, cursor_position);
            pos = a;
            size = b - a + 1;
        }
    }

    b_quads_sticky.clear();
    b_quads_sticky.push(pos, size, cursor_color);
    b_quads_sticky.update();

    set_dirty();
}

void grid_world::h_mouse_motion(SDL_MouseMotionEvent motion)
{
    auto [x, y] = sdl::nds_coords(*window, motion.x, motion.y);
    cursor_position = floor(glm::vec2(x, y) * glm::vec2(viewport_size / 2u));
    update_cursor();
}

void grid_world::h_mouse_down(SDL_MouseButtonEvent button)
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

void grid_world::h_mouse_up(SDL_MouseButtonEvent button)
{
    switch (button.button) {
    case SDL_BUTTON_LEFT: {
        if (auto select = get_mode<grid_selection>()) {
            if (auto& origin = select->drag_origin) {
                auto [a, b] = Rxt::box(*origin, cursor_position);
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
void grid_world::h_edge_scroll()
{
    // (0,0) is center-screen, so offset it to the corner
    grid_offset offset_pos = cursor_position + grid_offset(viewport_size / 2u);
    grid_offset dv {0};
    for (unsigned i = 0; i < dv.length(); ++i) {
        if (offset_pos[i] == 0) {
            dv[i] = -1;
        } else if (offset_pos[i] + 1 == (int)viewport_size[i]) {
            dv[i] = +1;
        }
    }
    if (dv != grid_offset{0}) {
        h_move_viewport(dv.x, dv.y);
    }
}

void grid_world::handle(SDL_Event event)
{
    switch (event.type) {
    case SDL_QUIT: { should_quit(true); return; }
    case SDL_KEYDOWN: { keys.press(event.key.keysym); break; }
    case SDL_MOUSEMOTION: { h_mouse_motion(event.motion); break; }
    case SDL_MOUSEBUTTONDOWN: { h_mouse_down(event.button); break; }
    case SDL_MOUSEBUTTONUP: { h_mouse_up(event.button); break; }
    }
}
