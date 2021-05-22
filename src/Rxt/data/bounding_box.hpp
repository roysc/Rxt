#pragma once

#include <Rxt/vec.hpp>
#include <Rxt/_fmt.hpp>

#include <utility>

namespace Rxt
{
template <class FT>
struct bounding_box
{
    using vec_type = vec::tvec2<FT>;
    vec_type m_min, m_max;

    bounding_box(vec_type a, vec_type b)
        : m_min(min(a, b))
        , m_max(max(a, b))
    {}

    bounding_box(std::pair<vec_type, vec_type> pair)
        : bounding_box(pair.first, pair.second) {}

    bounding_box() : bounding_box(vec_type(), vec_type()) {}

    static bounding_box with_length(vec_type a, vec_type l)
    {
        return bounding_box(a, a + l);
    }

    auto min_corner() const { return m_min; }
    auto max_corner() const { return m_max; }
    auto left() const { return min_corner(); }
    auto right() const { return max_corner(); }

    auto shape() const { return m_max - m_min; }
    auto center() const { return m_min + shape() / FT(2); }

    bool contains(const bounding_box& that) const
    {
        return all(
            lessThanEqual(min_corner(), that.min_corner()) &&
            lessThanEqual(that.max_corner(), max_corner())
        );
    }

    bool contains(vec_type point) const
    {
        return all(
            lessThanEqual(min_corner(), point) &&
            lessThanEqual(point, max_corner())
        );
    }

    bool intersects(const bounding_box& that) const
    {
        return !any(
            greaterThan(min_corner(), that.max_corner()) ||
            greaterThan(that.min_corner(), max_corner())
        );
    }
};

template <class FT>
bounding_box(vec::tvec2<FT>, vec::tvec2<FT>) -> bounding_box<FT>;
}

template <class FT>
struct fmt::formatter<Rxt::bounding_box<FT>>
{
    template <class PC>
    constexpr auto parse(PC& ctx)
    {
        return ctx.begin();
    }

    template <class FC>
    auto format(const Rxt::bounding_box<FT>& val, FC& ctx)
    {
        return fmt::format_to(ctx.out(), "bounding_box({}, {})", val.min_corner(), val.max_corner());
    }
};
