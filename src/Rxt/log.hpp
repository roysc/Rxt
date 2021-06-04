#pragma once
#include "format.hpp"

#define RXT_log(...) (::Rxt::print(__VA_ARGS__))

// TODO
#define RXT_error RXT_log
#define RXT_warn RXT_log
#define RXT_info RXT_log
#define RXT_debug RXT_log

#define RXT_show(var_) RXT_log(#var_ " = {}\n", (var_))
