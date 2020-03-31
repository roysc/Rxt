#pragma once

#include <CGAL/boost/graph/helpers.h>
#include <CGAL/boost/graph/copy_face_graph.h>

#include <boost/property_map/property_map.hpp>
#include <unordered_map>
#include <cassert>

namespace Rxt
{
// For mapping transformed faces back to source faces after transformation
template <class Source, class Target>
struct transform_comap_faces
{
    using source_mesh = Source;
    using target_mesh = Target;

    using GTSource = boost::graph_traits<source_mesh>;
    using GTTarget = boost::graph_traits<target_mesh>;
    using source_face = typename GTSource::face_descriptor;
    using target_face = typename GTTarget::face_descriptor;
    using source_halfedge = typename GTSource::halfedge_descriptor;
    using target_halfedge = typename GTTarget::halfedge_descriptor;

    using FMap = std::unordered_map<source_face, target_face>;
    using FComap = std::unordered_map<target_face, source_face>;
    using HMap = std::unordered_map<source_halfedge, target_halfedge>;
    using HComap = std::unordered_map<target_halfedge, source_halfedge>;

    using face_comap = FComap;
    using transform_function = bool (*)(target_mesh&);

    transform_function transformation {};

    transform_comap_faces(transform_function f) : transformation{f} {}

    // Paint source face comapping for new faces:
    // Walk halfedges to find new halfedge; recurse on face of the new opposite halfedge.
    template <class Faces>
    static bool set_source_face_aux(target_face fd, target_mesh const& m, Faces&& chain,
                                    FComap& fcomap, HComap& hcomap)
    {
        if (auto fi = fcomap.find(fd); fi != end(fcomap)) {
            for (auto sib_face: chain) // map all sibling faces to source
                fcomap.emplace(sib_face, fi->second);
            return true;
        }
        // find an incident halfedge with no comapping; fail if none
        auto h = halfedge(fd, m), h_end = h;
        while (hcomap.find(h) != end(hcomap)) {
            h = next(h, m);
            if (h == h_end) return false;
        }
        // continue searching on adjacent face
        chain.push_back(fd);
        auto fd_opp = face(opposite(h, m), m);
        return set_source_face_aux(fd_opp, m, chain, fcomap, hcomap);
    }

    static void set_source_face(target_face fd, target_mesh const& m, FComap& fcomap, HComap& hcomap)
    {
        assert(set_source_face_aux(fd, m, std::vector<target_face>{}, fcomap, hcomap));
    }

    face_comap operator()(source_mesh const& src, target_mesh& tgt)
    {
        FMap fmap;
        HMap hmap;
        auto params = CGAL::parameters::
            halfedge_to_halfedge_map(boost::make_assoc_property_map(hmap)).
            face_to_face_map(boost::make_assoc_property_map(fmap));
        CGAL::copy_face_graph(src, tgt, params);

        // Invert our mappings
        FComap fcomap;
        HComap hcomap;
        for (auto [s, t]: fmap) fcomap.emplace(t, s);
        for (auto [s, t]: hmap) hcomap.emplace(t, s);
        // Do transformation
        transformation(tgt);

        for (auto fd: faces(tgt)) {
            set_source_face(fd, tgt, fcomap, hcomap);
        }
        return fcomap;
    }
};
}
