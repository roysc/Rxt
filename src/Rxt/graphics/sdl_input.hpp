#pragma once

#include "../_debug.hpp"
#include <SDL2/SDL.h>
#include <fmt/format.h>
#include <utility>
#include <map>
#include <memory>
#include <string_view>
#include <functional>

namespace Rxt::sdl
{
constexpr char const* keymod_name(SDL_Keymod km);
constexpr int parse_keymods(std::string_view sv);
int conflate_modifiers(int mod);
std::string repr_keymods(int mods);

struct key_descriptor
{
    SDL_Keycode code;
    int mod;

    key_descriptor(SDL_Keycode c, int m)
        : code(c), mod(conflate_modifiers(m)) {}
    key_descriptor(SDL_Keysym k) : key_descriptor(k.sym, k.mod) {}
    key_descriptor(SDL_Keycode c) : key_descriptor(c, {}) {}

    // Specify by key name, e.g. "Up", "Right", "S-Left", "a", "C-b", "1", etc.
    key_descriptor(const char* s);

    friend bool operator<(const key_descriptor& lhs, const key_descriptor& rhs);
};

template <bool echo>
struct _key_dispatcher
{
    using callback = std::function<void()>;
    using fallback = std::function<void(key_descriptor)>;
    using callback_map = std::map<key_descriptor, callback>;

    callback_map on_press;
    callback_map on_scan;

    ~_key_dispatcher() {}        // fixme: breaks on clang?

    void press(SDL_Keysym keysym)
    {
        if constexpr (echo) {
            dbg::print("press: {}\n", key_descriptor(keysym));
        }
        auto b = on_press.find(keysym);
        if (b != on_press.end()) {
            b->second();
        }
    }

    void scan(fallback default_func = {})
    {
        auto const* state = SDL_GetKeyboardState(nullptr);
        int mod = conflate_modifiers(SDL_GetModState());

        for (auto&& [k, func]: on_scan) {
            auto scancode = SDL_GetScancodeFromKey(k.code);
            if (state[scancode] && (mod == k.mod)) {
                if constexpr (echo) { dbg::print("scan: {}\n", k); }
                func();
            }
            else if (default_func) {
                default_func(k);
            }
        }
    }
};

using key_dispatcher = _key_dispatcher<false>;
}

namespace fmt
{
using Rxt::sdl::key_descriptor;

template <>
struct formatter<key_descriptor> {
    template <class ParseContext>
    constexpr auto parse(ParseContext& ctx) { return ctx.begin(); }

    template <class FormatContext>
    auto format(const key_descriptor& km, FormatContext& ctx) {
        std::string mod;
        if (km.mod != KMOD_NONE) {
            mod = Rxt::sdl::repr_keymods(km.mod);
            mod += '-';
        }
        return format_to(ctx.out(), R"("{0}{1}")", mod, SDL_GetKeyName(km.code));
    }
};
}
