#pragma once

#include <Rxt/vec.hpp>
#include <utility>

namespace Rxt
{
template <class FT>
struct bounding_box
{
    using vec_type = vec::tvec2<FT>;
    vec_type _min, _max;

    bounding_box(vec_type a, vec_type b)
        : _min(min(a, b))
        , _max(max(a,b ))
    {}

    bounding_box(std::pair<vec_type, vec_type> pair)
        : bounding_box(pair.first, pair.second) {}

    bounding_box() : bounding_box(vec_type(), vec_type()) {}

    auto shape() const { return _max - _min; }
    auto center() const { return _min + shape() / FT(2); }

    auto left() const { return _min; }
    auto right() const { return _max; }

    bool contains(const bounding_box& box) const
    {
        return all(lessThanEqual(left(), box.left())) && all(lessThanEqual(box.right(), right()));
    }

    bool intersects(const bounding_box& box) const
    {
        return !any(greaterThanEqual(left(), box.right()) || greaterThanEqual(box.left(), right()));
    }
};

template <class FT>
bounding_box(vec::tvec2<FT>, vec::tvec2<FT>) -> bounding_box<FT>;
}
