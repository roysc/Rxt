#pragma once

#include "sdl_core.hpp"
#include "_sdl_debug.hpp"
#include <thread>

namespace Rxt::sdl
{
// Wraps a thread which pushes a custom SDL event at a specified frequency
struct _metronome
{
    unsigned m_event_type;
    std::thread m_thread;

    // _metronome(Dur dur, std::invocable auto pred)
    template <class Dur, class Pred>
    _metronome(Dur dur, Pred pred)
        : m_event_type{SDL_RegisterEvents(1)}
    {
        if (m_event_type == (unsigned)-1) {
            log_and_fail("SDL_RegisterEvents");
        }

        m_thread = std::thread([=, this] {
            SDL_Event event;
            SDL_zero(event);
            event.type = m_event_type;

            while (pred()) {
                SDL_PushEvent(&event);
                std::this_thread::sleep_for(dur);
            }
        });
    }

    ~_metronome() { m_thread.join(); }
};

#ifndef __EMSCRIPTEN__
using metronome = _metronome;
#else
// TODO figure out threads in wasm
struct metronome { template <class... T> metronome(T&&...) {} };
#endif
}
