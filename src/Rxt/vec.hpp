#pragma once

#include "_glm.hpp"

namespace Rxt
{
namespace vec
{
using namespace glm;

template <int L, class T>
using basic_vec = glm::vec<L, T, glm::highp>;

template <int L, class T>
constexpr auto squared_length(basic_vec<L, T> const& v)
{
    T sum = 0;
    for (int i{}; i < L; ++i)
        sum += v[i] * v[i];
    return sum;
}

template <int L, class T>
constexpr auto product(basic_vec<L, T> const& v)
{
    T prod = 1;
    for (int i{}; i < L; ++i)
        prod *= v[i];
    return prod;
}

template <int L, class T>
constexpr auto unravel_index(unsigned ix, basic_vec<L, T> const& shape)
{
    if constexpr (L == 1) {
        return basic_vec<L,T>(ix);
    } else {
        // shape excluding last axis
        basic_vec<L-1,T> slice_shape(shape);
        auto slice_area = product(slice_shape);
        return basic_vec<L,T>(
            unravel_index(ix % slice_area, slice_shape),
            ix / slice_area
        );
    }
}
}
}
