#pragma once

#include <utility>

namespace Rxt
{
inline namespace util
{
template <class T>
auto ordered(T a, T b)
{
    using std::swap;
    auto a_ = min(a, b);
    auto b_ = max(a, b);
    return std::make_pair(a_, b_);
}
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
}
