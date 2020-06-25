#pragma once
#include "io.hpp"

#define RXT_log(...) (::Rxt::print(__VA_ARGS__))
#define RXT_show(var_) RXT_log( #var_ " = {}\n", (var_))
