#pragma once

#include <Rxt/iterator.hpp>
#include <Rxt/geometry/helper.hpp>

#include <CGAL/Point_3.h>
#include <CGAL/Triangle_3.h>

#include <functional>

namespace Rxt
{
template <class MeshIter, class DescIter, auto get_face_range>
struct mesh_iterator_adaptor
{
    using mesh_iterator = MeshIter;
    using mesh = typename mesh_iterator::value_type;
    using descriptor_iterator = Rxt::chain_iterator<MeshIter, DescIter, get_face_range>;
};

template <class K, class Iter>
struct triangle_primitive
{
    // Interface types
    using Id = Iter;
    using Point = CGAL::Point_3<K>;
    using Datum = CGAL::Triangle_3<K>;

    // Primitive must allow access to mesh and face
    Id prim_iter;
    // Must be constructible from iterator passed to AABB_tree
    triangle_primitive(Id i)
        : prim_iter{i} {}

    Id id() const { return prim_iter; }

    Datum datum() const
    {
        auto [meshp, fdp] = prim_iter;
        auto [p0, p1, p2] = face_vertex_points<3>(*meshp, *fdp);
        return {p0, p1, p2};
    }

    Point reference_point() const
    {
        auto [meshp, fdp] = prim_iter;
        return face_vertex_points<1>(*meshp, *fdp)[0];
    }
};
}
