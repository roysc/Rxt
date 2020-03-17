#pragma once

#include "kernel.hpp"
#include "between.hpp"
#include "debug.hpp"

#include <CGAL/Kd_tree_rectangle.h>
#include <CGAL/squared_distance_3.h>

namespace Rxt::geom
{
// template <class P, auto get_point>
// template <class P, Point_3 (*get_point)(P const&)>
template <class K, class Key>
struct apparent_distance
{
    // Interface types for SearchTraits
    // https://doc.cgal.org/latest/Spatial_searching/classSearchTraits.html
    using FT = typename K::FT;
    using Point_d = Key;
    using Query_item = geom::Query_item;

    using rectangle = CGAL::Kd_tree_rectangle<FT, CGAL::Dimension_tag<3>>;
    using point_accessor = CGAL::Point_3<K> (*)(Point_d const&);

    point_accessor _get_point;

    Apparent_distance(point_accessor pa)
        : _get_point(pa) {}

    FT min_distance_to_rectangle(Query_item q, rectangle const& r) const
    {
        auto b = _to_box(r);
        auto seg = segment_between(q, b);
        return seg.squared_length();
    }

    FT max_distance_to_rectangle(Query_item q, rectangle const& r) const
    {
        using limits = std::numeric_limits<FT>;
        return limits::infinity();
    }

    FT transformed_distance(FT d) const { return d*d; }

    FT transformed_distance(Query_item q, Point_d key) const
    {
        auto p = get_point(key);
        auto qp = q.supporting_line().projection(p);
        return squared_distance(qp, p);
    }

    FT inverse_of_transformed_distance(FT d) const { return std::sqrt(d); }

    FT new_distance(FT dist, FT old_off, FT new_off, int cutting_dimension) const
    {
        return dist + (new_off * new_off) - (old_off * old_off);
    }

    static CGAL::Iso_cuboid_3<K> _to_box(rectangle const& r)
    {
        Point_3 bmin{r.min_coord(0), r.min_coord(1), r.min_coord(2)};
        Point_3 bmax{r.max_coord(0), r.max_coord(1), r.max_coord(2)};
        return Iso_box_3{bmin, bmax};
    }
};
}
