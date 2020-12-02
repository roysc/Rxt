#include "Rxt/util.hpp"
#include "Rxt/graphics/sdl.hpp"
#include "Rxt/color.hpp"
#include "Rxt/graphics/shader/solid_color_3D.hpp"

#include <exception>
#include <iostream>

using namespace Rxt;

struct loop_state : public sdl::simple_gui
                  , public Rxt::runtime
{
    using shader = Rxt::shader_programs::solid_color_3D<GL_LINE_STRIP>;
    bool dirty = true;
    sdl::key_dispatcher keys;
    shader line_prog;
    shader::buffers b_lines{line_prog};

    loop_state()
        : simple_gui("demo: sdl_shader", 500, 500)
    {
        keys.on_press["C-W"] = [this] { is_stopped(true); };

        auto color = Rxt::rgba{Rxt::colors::red, 1};
        auto points = {
            glm::vec3{-.5, -.5, 0},
            {-.5,  .5, 0},
            { .5,  .5, 0},
            { .5, -.5, 0},
            {-.5, -.5, 0}
        };
        auto& b = line_prog.buf["lines"];
        for (auto point: points) {
            b.push(point, color);
        }
        b.update();
        set(line_prog->mvp_matrix, glm::mat4{1});
    }

    void advance(SDL_Event event)
    {
        do {
            switch (event.type) {
            case SDL_QUIT: { is_stopped(true); return; }
            case SDL_KEYDOWN: { keys.press(event.key.keysym); break; }
            }
        } while (SDL_PollEvent(&event));

        if (dirty) {
            draw();
            dirty=(false);
        }
    }

    void draw()
    {
        glClearColor(0,0,0,0);
        glClear(GL_COLOR_BUFFER_BIT);
        line_prog.buf["lines"].draw();
        SDL_GL_SwapWindow(&window());
    }
};

extern "C" void step_loop(void* c)
{
    sdl::em_advance<loop_state>(c);
}

int main()
{
    auto loop = sdl::make_looper(new loop_state(), step_loop);
    loop();
}
