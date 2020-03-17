#pragma once

#include <utility>

namespace Rxt
{
template <class T>
std::pair<T, T> ordered(T a, T b)
{
    using std::swap;
    auto a_ = min(a, b);
    auto b_ = max(a, b);
    return std::make_pair(a_, b_);
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

auto get_second = [] (auto const& pair) { return pair.second; };
}
