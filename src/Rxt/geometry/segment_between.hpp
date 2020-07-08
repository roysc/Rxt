#pragma once

#include "util.hpp"

#include <CGAL/Segment_3.h>
#include <CGAL/Ray_3.h>
#include <CGAL/Iso_cuboid_3.h>

#include <cmath>
#include <limits>
#include <algorithm>

namespace Rxt
{
// Find the minimal segment between two segments
// FIXME still not ortho
template <class K>
CGAL::Segment_3<K> segment_between(CGAL::Segment_3<K> a, CGAL::Segment_3<K> b)
{
    using Seg = CGAL::Segment_3<K>;
    auto dot = [] (auto a, auto b) { return a * b; };

    // TODO: names come from formulas - improve
    auto ra = a.source(), rb = b.source(); // Point_3
    auto rab = (rb - ra);                  // Vector_3
    auto da = normalize(a.to_vector()), db = normalize(b.to_vector());
    typename K::FT t_a = 0, t_b = 0;

    auto denom = 1 - dot(da, db) * dot(da, db);
    // check if approx. parallel
    if (denom < _det::epsilon) {
        return Seg{ra, rb}; // arbitrary t
    } else {
        t_a = (dot(rab, da) - dot(rab, db) * dot(da, db)) / denom;
        t_b = - (dot(rab, db) - dot(rab, da) * dot(da, db)) / denom;
    }
    // TODO: TEST
    auto la = std::sqrt(a.squared_length());
    auto lb = std::sqrt(b.squared_length());
    t_a = std::clamp(t_a, 0.f, la);
    t_b = std::clamp(t_b, 0.f, lb);

    auto pa = ra + t_a * da, pb = rb + t_b * db; // Point_3
    return Seg{pa, pb};
}

// Find the minimal segment between a ray and a box
template <class K>
CGAL::Segment_3<K> segment_between(CGAL::Ray_3<K> q,
                                   CGAL::Iso_cuboid_3<K> r)
{
    using Seg = CGAL::Segment_3<K>;
    using FT = typename K::FT;

    using std::abs;
    using std::swap;
    using limits = std::numeric_limits<FT>;

    auto p = q.source();
    auto D = normalize(q.to_vector()); // Vector_3

    FT t_near = -limits::infinity(), t_far = limits::infinity();
    int i_near = -1, i_far = -1;

    for (int i = 0; i < 3; ++i) {
        FT b1 = r.min_coord(i), b2 = r.max_coord(i);

        // test for parallel
        if (abs(D[i]) < _det::epsilon) {
            // TODO test this!
            continue;
        }

        // Solve for t intersecting these planes
        auto t1 = (b1 - p[i]) / D[i];
        auto t2 = (b2 - p[i]) / D[i];
        if (t1 > t2)
            swap(t1, t2);

        if (t_near < t1) {
            t_near = t1;
            i_near = i;
        }
        if (t2 < t_far) {
            t_far = t2;
            i_far = i;
        }

        if (t_far < 0) {        // behind us?
            t_far = 0;
        }

        // No intersection
        // if (t_near > t_far && i == 2) {
    }

    auto p_near = p + D * t_near, p_far = p + D * t_far;

    // FIXME
    if (t_near > t_far) {
        // find the perpendicular axis
        int i_edge = (i_near + 1) % 3;
        if (i_edge == i_far) {
            i_edge += 1;
            i_edge %= 3;
        }

        auto boxmin = r.min(), boxmax = r.max(); // Point_3
        // Query segment
        Seg qseg{p_near, p_far};

        using _det::replacing;
        // Find closest edge corners
        auto c1 = boxmax;       // Point_3
        auto diffmin = boxmin - qseg.source();
        auto diffmax = boxmax - qseg.source();
        if (abs(diffmin[i_near]) < abs(diffmax[i_near]))
            c1 = replacing(c1, boxmin, i_near);
        if (abs(diffmin[i_far]) < abs(diffmax[i_far]))
            c1 = replacing(c1, boxmin, i_far);

        auto c2 = replacing(c1, boxmin, i_edge);
        // Edge segment
        Seg eseg{c1, c2};

        return segment_between(qseg, eseg);
    }

    return Seg{p_near, p_near};
}
}
