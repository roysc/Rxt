#include "grid_hash.hpp"
#include "Rxt/_testing.hpp"
#include "_test_util.hpp"

#include <algorithm>
#include <iterator>

namespace Rxt
{
inline int get_param() { return 10; }
auto get_box = [](auto* node) { return node->box; };

template <class FT>
void test_insert_and_query(FT cellsize)
{
    using Node = Node<FT>;

    auto n = get_param();
    auto nodes = generate_random_nodes<FT>(n, 20*cellsize, 2*cellsize);

    grid_hash_map<Node*, decltype(get_box), FT> map(vec::tvec2<FT>(cellsize), get_box);
    for (auto& node : nodes)
        map.insert(&node);

    auto intersections1 = std::vector<std::vector<Node*>>(nodes.size());
    for (const auto& node : nodes) {
        auto res = map.query(node.box);
        std::copy(res.begin(), res.end(), std::back_inserter(intersections1[node.id]));
        print("intersection contains:\n");
        for (auto node: res) {
            print("node: {}\n", node->id);
        }
    }

    auto intersections2 = std::vector<std::vector<Node*>>(nodes.size());
    for (const auto& node : nodes)
        intersections2[node.id] = brute_query(node.box, nodes, {});

    for (const auto& node : nodes)
        REQUIRE(equal_intersections(intersections1[node.id], intersections2[node.id]));
}

TEST_CASE("insert and query")
{
    test_insert_and_query<float>(0.1f);
    test_insert_and_query<unsigned>(1);
}
}
