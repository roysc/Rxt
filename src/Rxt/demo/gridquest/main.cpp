#include "main_context.hpp"

#include "Rxt/graphics/sdl.hpp"

namespace sdl = Rxt::sdl;

extern "C" void step_state(void* c)
{
    sdl::step<main_context>(c);
}

int main()
{
    grid_size world_size {32};
    glm::uvec2 tile_size_px {16};

    auto context = new main_context(world_size, tile_size_px);
    auto loop = sdl::make_looper(context, step_state);
    loop();

    return 0;
}
