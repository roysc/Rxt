#include "graph.hpp"
#include "Rxt/_testing.hpp"

#include <boost/graph/adjacency_list.hpp>

#include <set>
#include <type_traits>

namespace Rxt
{
namespace _det
{
using namespace boost;

using G = adjacency_list<
    vecS, vecS, undirectedS // , property<vertex_property_tag, int>
    >;
using GT = graph_traits<G>;
}

template <class A, class B>
bool equivalent(A const& a, B const& b)
{
    auto ai = begin(a);
    using Set = std::set<std::decay_t<decltype((*ai))>>;
    return Set{begin(a), end(a)} == Set{begin(b), end(b)};
}

TEST_CASE("adjacency table") {
    // create a 2-valent graph
    using namespace Rxt;
    using _det::G;
    using _det::GT;
    using VD = typename GT::vertex_descriptor;
    G g;
    VD v[4];

    add_edge(
        v[0] = add_vertex(g),
        v[1] = add_vertex(g),
        g
    );
    add_edge(
        v[2] = add_vertex(g),
        v[3] = add_vertex(g),
        g
    );
    add_edge(v[0], v[2], g);
    add_edge(v[1], v[3], g);

    // test with regular vector?
    using Row = std::array<VD,2>;
    std::vector<Row> t;
    // vector_property_map<VD> tm;
    create_adjacency_table<2>(g, t);

    REQUIRE( equivalent(t[v[0]], Row {v[1], v[2]}) );
}
}
