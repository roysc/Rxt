#pragma once

#include <Rxt/_debug.hpp>
#include <Rxt/range.hpp>

#include <boost/graph/graph_traits.hpp>
#include <vector>
#include <array>

namespace Rxt
{
// Create an adjacency table of all vertices of a K-valent graph
// where G is a graph (v, e)
// and R is a property map of (G.v -> v[3])
template <unsigned K, class Out, class G>
bool create_adjacency_table(const G& g, Out& out)
{
    using boost::graph_traits;

    using Vd = typename graph_traits<G>::vertex_descriptor;
    using Row = std::array<Vd, K>;

    auto ret = Out(num_vertices(g));
    auto vs = vertices(g);

    for (auto v: Rxt::to_range(vs)) {
        Row r;
        auto r_it = begin(r), r_end = end(r);
        for (auto [it, end] = adjacent_vertices(v, g);
             (it != end) && (r_it != r_end);
             ++it) {
            *r_it++ = *it;
        }
        if (r_it != r_end) {
            return false;
            // throw std::invalid_argument("graph is not K-valent");
        }
        ret[v] = r;
    }

    out = ret;
    return true;
}
}
