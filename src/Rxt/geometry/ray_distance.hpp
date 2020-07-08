#pragma once

#include "segment_between.hpp"

#include <CGAL/Kd_tree_rectangle.h>
#include <CGAL/squared_distance_3.h>
#include <limits>

namespace Rxt
{
template <class T, class K, auto get_point>
// requires(T const& a) { get_point(a) -> point }
struct ray_distance
{
    // CGAL distance interface
    using Point_d    = T;
    using D          = CGAL::Dimension_tag<3>;
    using Query_item = typename K::Ray_3;
    using FT         = typename K::FT;

    using Rect = CGAL::Kd_tree_rectangle<FT, D>;
    using Point = typename K::Point_3;//??

    static constexpr FT epsilon = 0.0001;

    FT min_distance_to_rectangle(Query_item q, Rect const& r) const
    {
        auto b = to_box<CGAL::Iso_cuboid_3<K>>(r);
        auto seg = segment_between(q, b);
        return seg.squared_length();
    }

    FT max_distance_to_rectangle(Query_item q, Rect const& r) const
    {
        using limits = std::numeric_limits<FT>;
        return limits::infinity();
    }

    FT transformed_distance(FT d) const { return d*d; }

    FT transformed_distance(Query_item q, Point_d pp) const
    {
        auto p = get_point(pp);
        auto qp = q.supporting_line().projection(p);
        return squared_distance(qp, p);
    }

    FT inverse_of_transformed_distance(FT d) const { return std::sqrt(d); }

    FT new_distance(FT dist, FT old_off, FT new_off, int cutting_dimension) const
    {
        return dist + (new_off * new_off) - (old_off * old_off);
    }

    template <class Box>
    static Box to_box(Rect const& r)
    {
        Point bmin{r.min_coord(0), r.min_coord(1), r.min_coord(2)};
        Point bmax{r.max_coord(0), r.max_coord(1), r.max_coord(2)};
        return Box(bmin, bmax);
    }
};
}
