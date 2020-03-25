#pragma once
#include "demangle.hpp"

#include <typeinfo>

namespace Rxt
{
const char* demangle(const char* name);

template <class T>
const char* demangle_type()
{
    return demangle(typeid(T).name());
}
}
