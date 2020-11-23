#pragma once

#include "_glm.hpp"
#include "_fmt.hpp"

#include <iostream>
#include <experimental/type_traits>

namespace Rxt
{
inline namespace vec
{
using namespace Rxt::_glm;
}
}

template <class T>
struct _fmt_glm_to_string
{
    template <class PC>
    constexpr auto parse(PC& ctx)
    {
        return ctx.begin();
    }

    template <class FC>
    auto format(const T& val, FC& ctx)
    {
        return fmt::format_to(ctx.out(), "{}", ::glm::to_string(val));
    }
};

template <class T>
struct fmt::formatter<glm::tvec2<T>> : _fmt_glm_to_string<glm::tvec2<T>> {};

template <class T>
struct fmt::formatter<glm::tvec3<T>> : _fmt_glm_to_string<glm::tvec3<T>> {};
