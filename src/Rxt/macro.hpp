#pragma once

#define RXT_def_ctors_move_only(name_)              \
    name_(const name_&) = delete;               \
    name_& operator=(const name_&) = delete;    \
    name_(name_&&) = default;                   \
    name_& operator=(name_&&) = default

#define RXT_set_lambda_nocap(val_, ...) (val_) = [](__VA_ARGS__)
#define RXT_set_lambda(val_, ...) (val_) = [&](__VA_ARGS__)
