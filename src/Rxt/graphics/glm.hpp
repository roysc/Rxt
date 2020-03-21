#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include <boost/predef.h>
#include <experimental/type_traits>
#include <iostream>
#include <type_traits>

namespace Rxt
{
#if !BOOST_COMP_GNUC
template <class T>
using glm_string_castable = decltype(glm::to_string(std::declval<T>()));
}

namespace std
{
template <class Ch, class Tr, class T,
          class = enable_if_t<std::experimental::is_detected_v<Rxt::glm_string_castable, T>>>
basic_ostream<Ch, Tr>& operator<<(basic_ostream<Ch, Tr>& o, T const& x)
{
    return o << glm::to_string(x);
}
#endif
}
