#include "Rxt/graphics/sdl.hpp"

#include <iostream>
#include <utility>

using namespace Rxt;

using CursorNds = std::pair<float, float>;
using Cursor = std::pair<unsigned short, unsigned short>;

struct loop_state
{
    sdl::key_dispatcher kd;
    int count = 0;
    bool _quit = false;
    Cursor cursor_position;
    sdl::window_ptr window;

    loop_state(sdl::window_ptr w) : window(w) {}
    void handle(SDL_Event);
    bool should_quit() const { return _quit; } // fixme
    void advance(SDL_Event);
};

extern "C" void step_loop(void* c)
{
    sdl::em_advance<loop_state>(c);
}

int main()
{
    const unsigned width = 500, height = 500;
    auto window = sdl::make_shared(
        SDL_CreateWindow("demo: sdl_events",
                         SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                         width, height,
                         SDL_WINDOW_OPENGL));
    sdl::gl_context _gl(window);

    auto* demo = new loop_state {window};
    sdl::emscripten_looper<loop_state> loop(demo, step_loop);
    loop();
}

void loop_state::advance(SDL_Event event)
{
    handle(event);

    SDL_GL_SwapWindow(window.get());
}

template <class T>
void dump(const char* m, Cursor c, T const&)
{
    auto [x,y] = c;
    std::cout << m << ": (" << x << ", " << y << ")\n";
}

void loop_state::handle(SDL_Event sdl_event)
{
    switch (sdl_event.type) {
    case SDL_QUIT: {
        _quit = true;
        return;
    }
    case SDL_MOUSEMOTION: {
        int x = sdl_event.motion.x, y = sdl_event.motion.y;
        cursor_position = {x, y};
        // cursor_position_nds = sdl::nds_coords(x, y, *window);
        break;
    }

    case SDL_MOUSEBUTTONDOWN: {
        dump("mouse_down", cursor_position, sdl_event.button.button);
        break;
    }

    case SDL_MOUSEBUTTONUP: {
        dump("mouse_up", cursor_position, sdl_event.button.button);
        break;
    }

    case SDL_KEYDOWN: {
        dump("key_down", cursor_position, sdl_event.key.keysym);
        break;
    }

    default: {}
    }
}
