#pragma once

#include "Rxt/io.hpp"
#include "sdl_core.hpp"

#include <utility>
#include <map>
#include <memory>
#include <string_view>
#include <functional>
#include <iostream>

namespace Rxt::sdl
{
char const* keymod_name(SDL_Keymod km);
int parse_keymods(std::string_view sv);
int simplify_modifiers(int mod);
std::string repr_keymods(int mods);

struct key_descriptor
{
    SDL_Keycode code;
    int mod;

    key_descriptor(SDL_Keycode c, int m)
        : code(c), mod(simplify_modifiers(m)) {}
    key_descriptor(SDL_Keysym k) : key_descriptor(k.sym, k.mod) {}
    key_descriptor(SDL_Keycode c) : key_descriptor(c, {}) {}

    // Specify by key name, e.g. "Up", "Right", "S-Left", "a", "C-b", "1"
    key_descriptor(const char* s);

    friend bool operator<(const key_descriptor& lhs, const key_descriptor& rhs);
};

struct key_dispatcher
{
    static constexpr bool echo = true;

    using callback = std::function<void()>;
    using fallback = std::function<void(key_descriptor)>;
    using callback_map = std::map<key_descriptor, callback>;

    callback_map on_press;
    callback_map on_scan;

    ~key_dispatcher() {}        // fixme: breaks on clang?

    void press(SDL_Keysym keysym)
    {
        if constexpr (echo) {
            print("press: {}\n", key_descriptor(keysym));
        }
        auto b = on_press.find(keysym);
        if (b != on_press.end()) {
            b->second();
        }
    }

    void scan(fallback default_func = {})
    {
        auto const* state = SDL_GetKeyboardState(nullptr);
        int mod = simplify_modifiers(SDL_GetModState());

        for (auto&& [k, func]: on_scan) {
            auto scancode = SDL_GetScancodeFromKey(k.code);
            if (state[scancode] && (mod == k.mod)) {
                if constexpr (echo) { print("scan: {}\n", k); }
                func();
            }
            else if (default_func) {
                default_func(k);
            }
        }
    }
};
}

namespace std
{
template <class Ch, class Tr>
basic_ostream<Ch, Tr>& operator<<(basic_ostream<Ch, Tr>& o, Rxt::sdl::key_descriptor const& kd)
{
    if (kd.mod != KMOD_NONE)
        o << Rxt::sdl::repr_keymods(kd.mod) << '-';
    return o << SDL_GetKeyName(kd.code);
}
}
