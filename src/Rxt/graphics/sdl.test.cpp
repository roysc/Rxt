#include "sdl.hpp"
#include "../_testing.hpp"
#include <chrono>

namespace Rxt::sdl
{
struct ctx : simple_gui
{
    ctx() : simple_gui("test", 10,10) {}
    bool is_stopped() const { return true; }
    void advance(SDL_Event e) {}
};
extern "C" void step(void* c)
{
    sdl::em_advance<ctx>(c);
}
using test_ctx = ctx;

TEST_CASE("simple_gui")
{
    auto loop = sdl::em_looper(new test_ctx(), step);
    loop();
}

struct ctx_met : test_ctx
{
    using dur = std::chrono::duration<float, std::ratio<1, 30>>;
    metronome m2{dur(), [this] { return !is_stopped(); }};
};

TEST_CASE("metronome")
{
    auto loop = sdl::em_looper(new ctx_met(), step);
    loop();
}
}
