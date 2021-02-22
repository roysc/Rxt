#pragma once

#include "_fmt.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/hash.hpp>

#include <type_traits>

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
