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
    using handler = std::function<void()>;
    using handler_map = std::map<key_descriptor, handler>;
    using fallback_handler = std::function<void(key_descriptor)>;

    handler_map on_press;
    handler_map on_scan;
    bool echo = false;

    fallback_handler m_fallback = [](key_descriptor key) { print("unhandled: {}\n", key); };

    void press(SDL_Keysym keysym)
    {
        // Defer to binding for held key
        if (on_scan.contains(keysym)) {
            return;
        }
        if (echo) {
            print("press: {}\n", key_descriptor(keysym));
        }
        if (auto it = on_press.find(keysym); it != on_press.end()) {
            it->second();
        } else if (m_fallback) {
            m_fallback(keysym);
        }
    }

    void scan()
    {
        auto state = SDL_GetKeyboardState(nullptr);
        int mod = simplify_modifiers(SDL_GetModState());

        for (auto&& [k, func]: on_scan) {
            auto scancode = SDL_GetScancodeFromKey(k.code);
            if (state[scancode] && (mod == k.mod)) {
                if (echo) { print("scan: {}\n", k); }
                func();
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
