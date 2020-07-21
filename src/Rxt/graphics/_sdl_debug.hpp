#pragma once

#include "sdl_core.hpp"
#include <exception>

namespace Rxt::sdl
{
inline void log_and_fail(const char* what, int r=0)
{
    auto msg = SDL_GetError();
    if (r) SDL_Log("%s: (%d) %s", what, r, msg);
    else SDL_Log("%s: %s", what, msg);
    throw std::runtime_error(msg);
}

}
