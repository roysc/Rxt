#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/hash.hpp>

#include <type_traits>

namespace Rxt
{
namespace _glm  = ::glm;

namespace _det
{
// fixme: unused
template <class T>
using has_glm_to_string = decltype(::glm::to_string(std::declval<T const&>()));
}
}
