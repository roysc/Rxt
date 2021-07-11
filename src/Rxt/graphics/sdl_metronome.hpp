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
    unsigned m_tick = 0;

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
                event.user.data1 = reinterpret_cast<void*>(m_tick++);
                SDL_PushEvent(&event);
                std::this_thread::sleep_for(dur);
            }
        });
    }

bool get_event_tick(SDL_Event e, unsigned* out) const
    {
        Rxt::print("metronome tick = {}\n", m_tick);
        if (e.type != m_event_type)
            return false;
        *out = static_cast<unsigned>(reinterpret_cast<unsigned long>(e.user.data1));
        return true;
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
