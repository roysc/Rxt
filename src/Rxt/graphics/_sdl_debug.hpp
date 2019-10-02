#pragma once

#include "sdl_prelude.hpp"
#include <exception>

namespace Rxt::sdl
{
inline void log_and_fail(const char* what)
{
    auto msg = SDL_GetError();
    SDL_Log("%s: %s", what, msg);
    throw std::runtime_error(msg);
}

}
