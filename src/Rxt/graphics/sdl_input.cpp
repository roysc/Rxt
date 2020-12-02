#include "sdl_input.hpp"
#include "../_debug.hpp"

#include <regex>

namespace Rxt::sdl
{
char const* keymod_name(SDL_Keymod km)
{
    switch (km) {
    case KMOD_LSHIFT: return "KMOD_LSHIFT";
    case KMOD_RSHIFT: return "KMOD_RSHIFT";
    case KMOD_LCTRL:  return "KMOD_LCTRL";
    case KMOD_RCTRL:  return "KMOD_RCTRL";
    case KMOD_RALT:   return "KMOD_RALT";
    case KMOD_LALT:   return "KMOD_LALT";
    case KMOD_RGUI:   return "KMOD_RGUI";
    case KMOD_LGUI:   return "KMOD_LGUI";
    case KMOD_NUM:    return "KMOD_NUM";
    case KMOD_CAPS:   return "KMOD_CAPS";
    case KMOD_MODE:   return "KMOD_MODE";
    case KMOD_NONE:   return "KMOD_NONE";
    default:          return "(?)";
    }
}

int simplify_modifiers(int mod)
{
    // Ignore L/R distinctions
    for (auto both: {KMOD_SHIFT, KMOD_CTRL, KMOD_ALT, KMOD_GUI}) {
        if (mod & both) mod |= both;
    }
    // Ignore others
    mod &= ~(KMOD_NUM | KMOD_CAPS | KMOD_MODE);
    return mod;
}

std::string repr_keymods(int mods)
{
    static const std::pair<int, char> modchars[] = {
        {KMOD_SHIFT, 'S'},
        {KMOD_CTRL, 'C'},
        {KMOD_ALT, 'M'},
        {KMOD_GUI, 'G'}
    };

    std::string ret;
    for (auto [km, ch]: modchars) {
        if (mods & km) {
            ret += ch;
            // clear after reading so we know what's left
            mods &= (~km);
        }
    }
    if (mods != KMOD_NONE) ret += "?";
    return ret;
}

int parse_keymods(std::string_view sv)
{
    int ret = KMOD_NONE;
    for (auto c: sv) {
        switch (c) {
        case 'S': ret |= KMOD_SHIFT; break;
        case 'C': ret |= KMOD_CTRL; break;
        case 'M': ret |= KMOD_ALT; break;
        case 'G': ret |= KMOD_GUI; break;
        }
    }
    return ret;
}

key_descriptor::key_descriptor(const char* s)
{
    auto sub_to_sv = [] (std::csub_match const& sub) {
        return std::string_view{sub.first, static_cast<std::size_t>(sub.length())};
    };
    static const std::regex rx("(?:([SCMG])-)?([^\\s]+)", std::regex_constants::optimize);

    if (std::cmatch rx_match; std::regex_match(s, rx_match, rx)) {
        auto modr = rx_match[1], keyr = rx_match[2];
        int mod = parse_keymods(sub_to_sv(modr));
        SDL_Keycode key = SDL_GetKeyFromName(keyr.str().c_str());

        if (key == SDLK_UNKNOWN)
            throw std::invalid_argument(keyr);

        *this = key_descriptor {key, mod};
    } else {
        throw std::invalid_argument(s);
    }
}

bool operator<(const key_descriptor& lhs, const key_descriptor& rhs)
{
    return std::pair(lhs.code, lhs.mod) < std::pair(rhs.code, rhs.mod);
}
}

