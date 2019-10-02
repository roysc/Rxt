#include "Rxt/graphics/sdl.hpp"
#include <iostream>

using namespace Rxt;

struct loop_state
{
    sdl::key_dispatcher kd;
    int count = 0;

    loop_state(sdl::window_ptr) {}
    bool should_quit() const { return count > 10; }

    void step()
    {
        SDL_Event event;
        SDL_WaitEvent(&event);

        std::cout << count++ << "\n";
    }
};

extern "C" void step_loop(void* c)
{
    sdl::step<loop_state>(c);
}

int main()
{
    sdl::simple_gui gui("demo: easy_gl", 500, 500);
    auto loop = sdl::make_looper(new loop_state(gui.window), step_loop);
    loop();
}
