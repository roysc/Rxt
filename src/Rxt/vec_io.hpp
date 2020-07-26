#pragma once

#include "_glm.hpp"

#ifndef RXT_DISABLE_STD_OSTREAM
namespace std
{
// using Rxt::operators::operator<<;

template <class Ch, class Tr, class T,
          enable_if_t<
              !experimental::is_detected_v<Rxt::_det::has_stream_operator, Ch, Tr, T> &&
              experimental::is_detected_v<Rxt::_det::has_glm_to_string, T>
              >* = {}>
basic_ostream<Ch, Tr>& operator<<(basic_ostream<Ch, Tr>& o, T const& x)
{
    return o << ::glm::to_string(x);
}
}
#else
static_assert(false, "ostream operator definitions are disabled");
#endif
