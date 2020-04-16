#pragma once

#include "sdl_core.hpp"
#include "_sdl_debug.hpp"
#include <thread>

namespace Rxt::sdl
{
// Wraps a thread which pushes a custom SDL event at a specified frequency
struct metronome
{
    std::uint32_t tick_event_type;
    std::thread _thread;

    template <class Dur, class Pred>
    metronome(Dur dur, Pred&& pred)
        : tick_event_type{SDL_RegisterEvents(1)}
    {
        if (tick_event_type == -1) {
            log_and_fail("SDL_RegisterEvents");
        }

        _thread = std::thread{[=] {
            SDL_Event event;
            SDL_zero(event);
            event.type = tick_event_type;

            while (pred()) {
                SDL_PushEvent(&event);
                std::this_thread::sleep_for(dur);
            }
        }};
    }

    ~metronome() { _thread.join(); }
};
}
