#include <Rxt/graphics/sdl.hpp>

#include <exception>
#include <iostream>

using namespace Rxt;

struct loop_state
{
    int count = 0;

    loop_state(sdl::window_ptr) {}
    bool should_quit() const { return count >= 10; }

    void step(SDL_Event event)
    {
        std::cout << (1 + count++) << " steps\n";
        if (should_quit())
            std::cout << "done\n";
    }
};

extern "C" void step_loop(void* c)
{
    sdl::step<loop_state>(c);
}

int main()
{
    sdl::simple_gui gui("demo: sdl_basic", 500, 500);
    auto loop = sdl::make_looper(new loop_state(gui.window), step_loop);
    loop();
}
