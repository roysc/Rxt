#pragma once

#include <Rxt/_debug.hpp>
#include <Rxt/range.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <vector>
#include <array>

namespace Rxt
{
namespace _det
{
using boost::vecS;
using boost::directedS;
using boost::undirectedS;
using boost::directedS;
using boost::adjacency_list;

// using no_prop = no_property;
// template <class T>
// using v_prop = property<boost::vertex_property_tag, T>;
// template <class T>
// using e_prop = property<boost::edge_property_tag, T>;

template <class... P>
using directed_adj_list = adjacency_list<vecS, vecS, directedS, P...>;

// using g_dl = adjacency_list<vecS, vecS, directedS, P...>;
}

inline namespace data
{
using _det::directed_adj_list;
using boost::property;
using boost::no_property;
using boost::vertex_property_tag;
using boost::edge_property_tag;
}

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
