#pragma once

#include "Rxt/vec.hpp"

#include <utility>
#include <functional>

namespace Rxt
{
// sort the corners and return canonical form
template <class T>
std::pair<T, T> box(T a, T b)
{
    auto a_ = min(a, b);
    auto b_ = max(a, b);
    return std::make_pair(a_, b_);
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

auto get_first = [] (auto&& pair) -> auto&& { return (pair.first); };
auto get_second = [] (auto&& pair) -> auto&& { return (pair.second); };

// Function wrapper to batch calls and flush manually
struct lazy_action
{
    using action_function = std::function<void()>;

    action_function function;
    unsigned count = 0;

    template <class F>
    lazy_action(F&& f) : function{f} {}

    void operator()() { ++count; }

    unsigned flush()
    {
        if (count && function) {
            function();
            auto ret = count;
            count = 0;
            return ret;
        }
        return 0;
    }
};

struct runnable //todo nix?
{
    bool _running = true;

    bool running() const { return _running; }
    void set_running(bool v) { _running = v; }

    virtual ~runnable() {}
};

// WIP https://gamedev.stackexchange.com/questions/151877/handling-variable-frame-rate-in-sdl2
template <class C>
void loop(C& ctx)
{
    // Uint32 time_step_ms = 1000 / fps_the_game_was_designed_for;
    auto next_step = Clock::now(); // initial value

    while(ctx.is_running()) {
        auto now =         Clock::now();

        // Check so we don't render for no reason (unless vsync is enabled)
        if (next_step <= now || vsync_enabled){

            // max number of advances per render, adjust this according to your minimum playable fps
            int computer_is_too_slow_limit = 10;

            // Loop until all steps are executed or computer_is_too_slow_limit is reached
            while ((next_step <= now) && (computer_is_too_slow_limit--)){
                advance(ctx);
                next_game_step += time_step_ms; // count 1 game tick done
            }

            render(ctx);
        } else {
            // we're too fast, wait a bit.
            if (be_nice_and_dont_burn_the_cpu) {
                SDL_Delay(next_game_tick - now);
            }
        }
    }    
}
}
