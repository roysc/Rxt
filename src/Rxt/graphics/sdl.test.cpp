#include "sdl.hpp"
#include "../_testing.hpp"

namespace Rxt::sdl
{
struct ctx : simple_gui
{
    ctx() : simple_gui("test", 10,10) {}
    friend bool should_quit(ctx const&) { return true; }
    friend void advance(ctx&, SDL_Event) {}
};

extern "C" void step(void* c)
{
    sdl::em_advance<ctx>(c);
}

TEST_CASE("simple_gui")
{
    auto loop = sdl::em_looper(new ctx(), step);
    loop();
}
}
