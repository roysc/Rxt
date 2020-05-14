#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <experimental/type_traits>
#include <iostream>
#include <type_traits>

namespace Rxt::_det
{
template <class T>
using has_glm_to_string = decltype(glm::to_string(std::declval<T const&>()));

template <class Ch, class Tr, class T>
using has_stream_operator =
    decltype(std::declval<std::basic_ostream<Ch, Tr>&>() << std::declval<T const&>());
}

// template <class S, class T>
// concept has_stream_operator =
// requires(S& o, T const& a) { { o << a; } -> S&; };

namespace std
{
template <class Ch, class Tr, class T,
          enable_if_t<
              !experimental::is_detected_v<Rxt::_det::has_stream_operator, Ch, Tr, T> &&
              experimental::is_detected_v<Rxt::_det::has_glm_to_string, T>
              >* = nullptr>
basic_ostream<Ch, Tr>& operator<<(basic_ostream<Ch, Tr>& o, T const& x)
{
    return o << glm::to_string(x);
}

// template <class S, class T>
// requires(S& o, T const& a) {
//     { ::glm::to_string(a) };
//     requires Rxt::detail::has_stream_operator<S, T>;
// }
// S& operator<<(S& o, T const& x) { return o << glm::to_string(x); }
}
