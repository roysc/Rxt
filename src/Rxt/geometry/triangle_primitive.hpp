#pragma once

#include "mesh_traits.hpp"

#include <Rxt/iterator.hpp>
#include <Rxt/geometry/helper.hpp>

#include <CGAL/Point_3.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/Kernel_traits.h>

#include <utility>

namespace Rxt
{
template <class MeshIter, class DescIter, auto get_face_range>
struct mesh_iterator_adaptor
{
    using mesh_iterator = MeshIter;
    using mesh = typename mesh_iterator::value_type;
    using descriptor_iterator = Rxt::chain_iterator<MeshIter, DescIter, get_face_range>;
};

template <class MeshContainer>
struct triangle_primitive
{
    using Mesh = typename MeshContainer::value_type;
    using Face = typename boost::graph_traits<Mesh>::face_descriptor;
    using K = typename CGAL::Kernel_traits<typename mesh_traits<Mesh>::point>::Kernel;

    // Interface types
    using Id = std::pair<typename MeshContainer::size_type, Face>;
    using Point = CGAL::Point_3<K>;
    using Datum = CGAL::Triangle_3<K>;
    using Point_reference = CGAL::Point_3<K> const&;
    using Datum_reference = CGAL::Triangle_3<K> const&;
    using Shared_data = MeshContainer const*;

    Id key;

    triangle_primitive(Id i) : key{i} {}

    Id id() const { return key; }

    Datum_reference datum(Shared_data const& meshes) const
    {
        auto [index, fd] = key;
        auto [p0, p1, p2] = face_vertex_points<3>(meshes->at(index), fd);
        return {p0, p1, p2};
    }

    Point_reference reference_point(Shared_data const& meshes) const
    {
        auto [index, fd] = key;
        return face_vertex_points<1>(meshes->at(index), fd)[0];
    }

    static Shared_data construct_shared_data(Shared_data const& meshes)
    {
        return meshes;
    }
};
}
