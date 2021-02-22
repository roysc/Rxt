#include "Rxt/graphics/sdl.hpp"

#include <exception>
#include <iostream>

using namespace Rxt;

struct loop_state
{
    int count = 0;

    loop_state(SDL_Window& win) {}
    bool is_stopped() const { return count >= 10; }

    void advance(SDL_Event)
    {
        std::cout << (1 + count++) << " steps\n";
        if (is_stopped())
            std::cout << "done\n";
    }
};

extern "C" void step_loop(void* c)
{
    sdl::em_advance<loop_state>(c);
}

int main()
{
    sdl::simple_gui gui("demo: sdl_basic", 500, 500);
    auto loop = sdl::make_looper(new loop_state(*gui.window()), step_loop);
    loop();
}
