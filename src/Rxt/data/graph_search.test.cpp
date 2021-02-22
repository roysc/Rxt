#include "../_testing.hpp"
#include "graph_search.hpp"
#include "grid_graph.hpp"
#include "Rxt/io.hpp"
#include "Rxt/vec.hpp"

#include <boost/property_map/function_property_map.hpp>

namespace Rxt
{
using vec::ivec2;
using Path = std::vector<ivec2>;

template <class Grid>
void test_search(Path expected_path)
{
    using Vertex = Grid::vertex_type;
    using Edge = Grid::edge_type;

    Grid grid{10, 10};
    Vertex start {1, 4}, goal{8,9};
    std::unordered_map<Vertex, Vertex> came_from;
    auto cost_map = boost::make_function_property_map<Edge>([](Edge e) {return 1;});

    auto heuristic = [](Vertex a, Vertex b) {
        auto diff = a - b;
        return diff.x + diff.y;
    };
    a_star_search(grid, start, goal, came_from, cost_map, heuristic);

    Path path = reconstruct_path(start, goal, came_from);
    REQUIRE(range_equal(expected_path, path));
}

TEST_CASE("a_star_search") {
    SUBCASE("clamped grid") {
        test_search<grid_graph_2d<>>({
            ivec2(1, 4),
            ivec2(1, 5),
            ivec2(2, 5),
            ivec2(2, 6),
            ivec2(2, 7),
            ivec2(3, 7),
            ivec2(4, 7),
            ivec2(4, 8),
            ivec2(5, 8),
            ivec2(6, 8),
            ivec2(6, 9),
            ivec2(7, 9),
            ivec2(8, 9),
        });
    }
    SUBCASE("wrapped grid") {
        test_search<grid_graph_2d<grid_boundary::wrap>>({
            ivec2(1, 4),
            ivec2(0, 4),
            ivec2(0, 3),
            ivec2(0, 2),
            ivec2(0, 1),
            ivec2(0, 0),
            ivec2(9, 0),
            ivec2(8, 0),
            ivec2(8, 9),
        });
    }
}
}
