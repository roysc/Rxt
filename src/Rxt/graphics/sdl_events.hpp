#pragma once

#include "_sdl_debug.hpp"
#include <thread>

namespace Rxt::sdl
{
template <class Dur, class Pred>
auto start_metronome(Dur dur, Pred p)
{
    auto tick_event_type = SDL_RegisterEvents(1);
    if (tick_event_type == -1) {
        sdl::log_and_fail("SDL_RegisterEvents");
    }

    auto func = [=] {
        SDL_Event event;
        SDL_zero(event);
        event.type = tick_event_type;

        while (p()) {
            SDL_PushEvent(&event);
            std::this_thread::sleep_for(dur);
        }
    };
    return std::thread(func);
}
}
