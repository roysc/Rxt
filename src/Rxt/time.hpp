#pragma once

#include <chrono>

namespace Rxt
{
template <unsigned N>
using duration_fps = std::chrono::duration<float, std::ratio<1, N>>;

// Convenient for ticking
template <class Clock, class Dur>
Dur advance(std::chrono::time_point<Clock,Dur>& last)
{
    auto current = Clock::now();
    Dur dt = current - last;
    last = current;
    return dt;
}
}
