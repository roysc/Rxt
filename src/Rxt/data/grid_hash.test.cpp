#include "grid_hash.hpp"
#include "Rxt/_testing.hpp"
#include "_test_util.hpp"

#include <algorithm>
#include <iterator>

namespace Rxt
{
inline int get_param() { return 10; }
auto get_box = [](auto* node) { return node->box; };

#define _run_testcase_FT(func_, FT_, maxsize_)                          \
    do {                                                                \
    using Box = bounding_box<FT_>;                                      \
    using Vec = typename Box::vec_type;                                 \
    auto n = get_param();                                               \
    auto box = Box(Vec(0.0f), Vec(100*maxsize_));                       \
    auto nodes = generate_random_nodes<FT_>(n, 100*maxsize_, maxsize_); \
    auto tree = Quadtree<FT_>(box, get_box);                            \
    grid_hash_map<Node<FT_>*, decltype(get_box), FT> map(vec::tvec2<FT_>(cellsize), get_box); \
    for (auto& node : nodes)                                            \
        map.insert(&node);                                              \
    func_<FT_>(std::move(map), nodes);                                  \
    } while(0)

#define _define_testcase(desc_, func_)              \
    TEST_CASE(desc_)                                \
    {                                               \
        SUBCASE("with float") {                     \
            _run_testcase_FT(func_, float, 0.01f);  \
        }                                           \
        SUBCASE("with uint") {                      \
            _run_testcase_FT(func_, unsigned, 10);  \
        }                                           \
    }

_define_testcase("quadtree: insert and query", test_insert_and_query)
_define_testcase("quadtree: insert and find all intersections", test_find_all)
_define_testcase("quadtree: insert remove and query", test_insert_remove_query)
_define_testcase("quadtree: insert remove and find all intersections", test_insert_remove_find_all)

// template <class FT>
// void test_insert_and_query(FT cellsize)
// {
//     using Node = Node<FT>;

//     auto n = get_param();
//     auto nodes = generate_random_nodes<FT>(n, 20*cellsize, 2*cellsize);

//     grid_hash_map<Node*, decltype(get_box), FT> map(vec::tvec2<FT>(cellsize), get_box);
//     for (auto& node : nodes)
//         map.insert(&node);

//     auto intersections1 = std::vector<std::vector<Node*>>(nodes.size());
//     for (const auto& node : nodes) {
//         auto res = map.query(node.box);
//         std::copy(res.begin(), res.end(), std::back_inserter(intersections1[node.id]));
//     }

//     auto intersections2 = std::vector<std::vector<Node*>>(nodes.size());
//     for (const auto& node : nodes)
//         intersections2[node.id] = brute_query(node.box, nodes, {});

//     for (const auto& node : nodes)
//         REQUIRE(equal_intersections(intersections1[node.id], intersections2[node.id]));
// }

// TEST_CASE("insert and query")
// {
//     test_insert_and_query<float>(0.1f);
//     test_insert_and_query<unsigned>(1);
// }
}
