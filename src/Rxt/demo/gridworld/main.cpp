#include "main_context.hpp"

#include "Rxt/graphics/sdl.hpp"
#include <chrono>

extern "C" void step_state(void* c)
{
    sdl::step<main_context>(c);
}

int main()
{
    glm::uvec2 world_size {32};
    glm::uvec2 tile_size_px {16};
    std::chrono::duration<float, std::ratio<1,30>> tick_dur(1);

    auto context = new main_context(world_size, tile_size_px);
    auto loop = sdl::make_looper(context, step_state);
    auto t = sdl::start_metronome(tick_dur, [&] { return !context->should_quit(); });

    loop();
    t.join();

    return 0;
}
