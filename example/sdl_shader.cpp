#include "Rxt/util.hpp"
#include "Rxt/graphics/sdl.hpp"
#include "Rxt/graphics/color.hpp"
#include "Rxt/graphics/shader/solid_color_3D.hpp"

#include <exception>
#include <iostream>

using namespace Rxt;

struct loop_state : public sdl::simple_gui
                  , public Rxt::runtime
{
    using shader = Rxt::shader_programs::solid_color_3D<GL_LINE_STRIP>;

    sdl::key_dispatcher keys;
    gl::program_loader loader;

    // grid_quad_2D quad_prog {loader};
    // grid_quad_2D::data b_quads {quad_prog};
    shader line_prog{loader};
    shader::data b_lines{line_prog};

    loop_state()
        : simple_gui("demo: sdl_shader", 500, 500)
    {
        keys.on_press["C-W"] = [this] { should_quit(true); };

        auto color = Rxt::rgba{Rxt::colors::red, 1};
        auto points = {
            glm::vec3{-.5, -.5, 0},
            {-.5,  .5, 0},
            { .5,  .5, 0},
            { .5, -.5, 0},
            {-.5, -.5, 0}
        };
        for (auto point: points) {
            b_lines.push(point, color);
        }
        b_lines.update();

        set(line_prog->mvp_matrix, glm::mat4{1});
    }

    void step(SDL_Event event)
    {
        do {
            switch (event.type) {
            case SDL_QUIT: { should_quit(true); return; }
            case SDL_KEYDOWN: { keys.press(event.key.keysym); break; }
            }
        } while (SDL_PollEvent(&event));

        if (is_dirty()) {
            draw();
            set_dirty(false);
        }
    }

    void draw()
    {
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT);
        b_lines.draw();
        SDL_GL_SwapWindow(window.get());
    }
};

extern "C" void step_loop(void* c)
{
    sdl::step<loop_state>(c);
}

int main()
{
    auto loop = sdl::make_looper(new loop_state(), step_loop);
    loop();
}
