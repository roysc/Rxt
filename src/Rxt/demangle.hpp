#pragma once

#include <typeinfo>
#include <string>
#include <cstring>

namespace Rxt
{
const char* demangle(const char* name);

template <class T>
const char* type_name()
{
    return demangle(typeid(T).name());
}

template <class T>
const char* type_name(const T&)
{
    return type_name<T>();
}
}
