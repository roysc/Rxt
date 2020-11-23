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
namespace _glm
{
using glm::fvec2;
using glm::ivec2;
using glm::uvec2;
using glm::fvec3;
using glm::ivec3;
using glm::uvec3;
using glm::fvec4;
using glm::ivec4;
using glm::uvec4;

using glm::quat;
using glm::fmat3;
using glm::fmat4;

using glm::tvec2;
using glm::tvec3;
using glm::tvec4;

using glm::highp;

using glm::translate;
using glm::scale;
}

namespace _det
{
// fixme: unused
template <class T>
using has_glm_to_string = decltype(::glm::to_string(std::declval<T const&>()));
}
}
