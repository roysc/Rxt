#pragma once

#include <type_traits>
#include <tuple>

namespace Rxt
{
template <unsigned size>
struct clamped_axis : public std::integral_constant<unsigned, size>
{
    static constexpr bool is_wrapped = false;
    static constexpr unsigned extent = size;
};

template <unsigned size>
struct wrapped_axis : public std::integral_constant<unsigned, size>
{
    static constexpr bool is_wrapped = true;
    static constexpr unsigned extent = size;
};

// Specifiies topological traits
// ie., number of dimensions and boundary wrap/clamp behavior
template <class... B>
struct grid_spec
{
    static constexpr unsigned dimension = sizeof...(B);

    static constexpr unsigned extent[dimension] {B::extent...};
    static constexpr bool is_wrapped[dimension] {B::is_wrapped...};
};

template <class Spec>
struct grid_coord
{
    std::array<unsigned, Spec::dimension> _arr;

    // friend grid_coord operator+(grid_offset<Spec> o)
    // friend grid_coord operator-(grid_offset<Spec> o)
};

template <class Spec>
struct grid_offset
{
    std::array<unsigned, Spec::dimension> _arr;

    // friend grid_offset operator+(grid_offset<Spec> o)
    // friend grid_offset operator-(grid_offset<Spec> o)
    // friend grid_offset operator*(grid_offset<Spec> o)
};
}
