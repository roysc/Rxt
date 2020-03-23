#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <experimental/type_traits>
#include <iostream>
#include <type_traits>

namespace Rxt
{
template <class T>
using has_glm_to_string = decltype(glm::to_string(std::declval<T>()));

template <class Ch, class Tr, class T>
using has_stream_operator =
    decltype(std::declval<std::basic_ostream<Ch, Tr>>() << std::declval<T>());
}

namespace std
{
template <class Ch, class Tr, class T,
          enable_if_t<
              !experimental::is_detected_v<Rxt::has_stream_operator, Ch, Tr, T> &&
              experimental::is_detected_v<Rxt::has_glm_to_string, T>
              >* = nullptr>
basic_ostream<Ch, Tr>& operator<<(basic_ostream<Ch, Tr>& o, T const& x)
{
    return o << glm::to_string(x);
}
}
