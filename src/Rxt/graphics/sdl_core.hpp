#pragma once

#include <SDL2/SDL.h>

#include <memory>

namespace Rxt::sdl
{
namespace _det
{
// https://blog.galowicz.de/2016/02/21/automatic_resource_release_with_sdl/
static void destroy(SDL_Window* r) { SDL_DestroyWindow(r); }
// static void destroy(SDL_Renderer* r) { SDL_DestroyRenderer(r); }
// static void destroy(SDL_Texture* r) { SDL_DestroyTexture(r);  }
// static void destroy(SDL_Surface* r) { SDL_FreeSurface(r);     }
}

template <typename T>
auto make_shared(T* t) {
    return std::shared_ptr<T>(t, [](T* t) { _det::destroy(t); });
}

using window_ptr = std::shared_ptr<SDL_Window>;
}
