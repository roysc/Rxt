#include "Rxt/io.hpp"
#include "Rxt/viewport.hpp"
#include "Rxt/controls.hpp"
#include "Rxt/color.hpp"

#include "Rxt/graphics/image.hpp"
#include "Rxt/graphics/sdl.hpp"
#include "Rxt/graphics/sdl/reactive.hpp"
#include "Rxt/graphics/gl.hpp"
#include "Rxt/graphics/sprite.hpp"

#include "Rxt/graphics/shader/grid_quad_texture_2D.hpp"
#include "Rxt/graphics/shader/grid_quad_2D.hpp"

// #include <nlohmann/json.hpp>

#include <functional>
#include <stdexcept>

using namespace Rxt;

using Rxt::shader_programs::grid_quad_texture_2D;
using Rxt::shader_programs::grid_quad_2D;
using vec::ivec2;
using vec::uvec2;
using viewport = adapt_reactive_template<reactive_viewport, hooks<>, int>;

// struct sprite
// {
//     uvec2 m_sheet_coord;
//     unsigned m_state;
// };

struct sprites_demo : sdl::simple_gui
{

    bool m_quit = false;
    sdl::key_dispatcher m_keys;
    sdl::input_hooks m_input;
    viewport m_viewport;
    // basic_cursor<ivec2> m_cursor;

    gl::file_asset_source m_assets;
    grid_quad_texture_2D m_sprites_prog;
    grid_quad_texture_2D::buffers m_sprites_bufs{m_sprites_prog};
    grid_quad_2D m_debug_prog;
    grid_quad_2D::buffers m_debug_bufs{m_debug_prog};

    sprites_demo(viewport, const char*);
    void advance(SDL_Event);
    void draw();
    bool is_stopped() { return m_quit; }

    // sprite_sheet load_sprite_sheet(const char* path, gl::texture&);

    void _init_controls();
};

extern "C" void step_state(void* c)
{
    sdl::em_advance<sprites_demo>(c);
}

int main(int argc, char** argv)
{
    using namespace Rxt::vec;

    auto texture_name = "sprite-test";
    if (argc > 1) {
        texture_name = argv[1];
    }

    viewport vp(ivec2(400));
    vp.set_scale(uvec2(8));
    auto demo = new sprites_demo(vp, texture_name);

    print("GL version: {}\n", glGetString(GL_VERSION));
    
    sdl::make_looper(demo, step_state)();
    return 0;
}

void sprites_demo::_init_controls()
{
    RXT_observe(m_input.on_quit) { m_quit = true; };
    RXT_observe(m_input.on_key_down, auto key) { m_keys.press(key); };

    auto move_viewport = [this] (int dx, int dy) { m_viewport.translate(ivec2(dx, dy)); };
    auto scale_viewport = [this] (int a) { m_viewport.set_scale(m_viewport.scale_pow(a)); };

    m_keys.on_press["C-W"] = [&] { m_quit = true; };

    m_keys.on_scan["S-Left"]  = std::bind(move_viewport, -1, 0);
    m_keys.on_scan["S-Right"] = std::bind(move_viewport, +1, 0);
    m_keys.on_scan["S-Down"]  = std::bind(move_viewport, 0, -1);
    m_keys.on_scan["S-Up"]    = std::bind(move_viewport, 0, +1);

    m_keys.on_press["."] = std::bind(scale_viewport, +1);
    m_keys.on_press[","] = std::bind(scale_viewport, -1);

    m_keys.on_press["v"] = [&] { print("{} (cells={})\n", m_viewport, m_viewport.size_cells()); };
}

sprites_demo::sprites_demo(viewport vp, const char* texture_name)
    : simple_gui("sprites demo", vp.size_pixels())
    , m_viewport(vp)
{
    _init_controls();

    RXT_observe(m_viewport.on_update) {
        set(m_sprites_prog->viewport_size, m_viewport.size_cells());
        set(m_sprites_prog->viewport_position, m_viewport.position());
        set(m_debug_prog->viewport_size, m_viewport.size_cells());
        set(m_debug_prog->viewport_position, m_viewport.position());
    };
    
    auto sheet = load_sprite_sheet(m_assets.find_texture(texture_name), m_sprites_bufs.texture());

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    auto bg = colors::russet;
    glClearColor(bg.r, bg.g, bg.b, 1);

    ivec2 corners[] = {
        ivec2(0),
        ivec2(0,1),
    };
    for (auto pos: corners) {
        m_sprites_bufs.push(
            // offset by (-1,-1), multiply by tile size
            ivec2(sheet.tile_size) * (pos - 1),
            sheet.sprite_position(pos),
            sheet.tile_size);
    }
    m_sprites_bufs.update();

    m_viewport.on_update();
}

void sprites_demo::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    m_sprites_bufs.draw();
    m_debug_bufs.draw();

    SDL_GL_SwapWindow(window());
}

void sprites_demo::advance(SDL_Event event)
{
    do {
        m_input.handle_input(event);
    } while (SDL_PollEvent(&event));

    m_keys.scan();

    auto updates = {
        &m_viewport.on_update,
    };
    auto dirty = Rxt::flush_all(updates);

    if (dirty) draw();
}
