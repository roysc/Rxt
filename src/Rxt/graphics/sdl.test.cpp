#include "sdl.hpp"
#include "../_testing.hpp"
#include <chrono>

extern "C" void step(void*);

namespace Rxt::sdl
{
struct TestCtx : simple_gui
{
    TestCtx() : simple_gui("test", 10,10) {}
    bool is_stopped() const { return true; }
    void advance(SDL_Event e) {}
};

TEST_CASE("simple_gui")
{
    auto loop = sdl::em_looper(new TestCtx(), step);
    loop();
}

struct TestCtxMet : TestCtx
{
    using dur = std::chrono::duration<float, std::ratio<1, 30>>;
    metronome m2{dur(), [this] { return !is_stopped(); }};
};

TEST_CASE("metronome")
{
    auto loop = sdl::em_looper(new TestCtxMet(), step);
    loop();
}
}

extern "C" void step(void* c)
{
    using namespace Rxt::sdl;
    em_advance<TestCtx>(c);
}
