#include "quadtree.hpp"
#include "Rxt/_testing.hpp"
#include "_test_util.hpp"

#include <utility>

namespace Rxt
{
inline int get_param() { return 100; }
auto get_box = [](auto* node) { return node->box; };
template <class FT>
using Quadtree = quadtree<Node<FT>*, decltype(get_box), FT>;

template <class FT>
void test_insert_and_query(Quadtree<FT> tree, std::vector<Node<FT>>& nodes)
{
    auto query_ixns = std::vector<std::vector<Node<FT>*>>(nodes.size());
    for (const auto& node : nodes)
        query_ixns[node.id] = tree.query(node.box);
    auto brute_ixns = std::vector<std::vector<Node<FT>*>>(nodes.size());
    for (const auto& node : nodes)
        brute_ixns[node.id] = brute_query(node.box, nodes, {});
    for (const auto& node : nodes)
        REQUIRE(equal_intersections(query_ixns[node.id], brute_ixns[node.id]));
}

template <class FT>
void test_find_all(Quadtree<FT> tree, std::vector<Node<FT>>& nodes)
{
    auto query_ixns = tree.find_all_intersections();
    auto brute_ixns = brute_find_all_intersections(nodes, {});
    REQUIRE(equal_intersections(query_ixns, brute_ixns));
}

template <class FT>
void test_insert_remove_query(Quadtree<FT> tree, std::vector<Node<FT>>& nodes)
{
    // Randomly remove some nodes
    auto rng = std::default_random_engine();
    auto death_dist = std::uniform_int_distribution(0, 1);
    auto removed = std::vector<bool>(nodes.size());
    std::generate(std::begin(removed), std::end(removed),
        [&rng, &death_dist](){ return death_dist(rng); }
    );
    for (auto& node : nodes)
    {
        if (removed[node.id])
            tree.erase(&node);
    }
    auto query_ixns = std::vector<std::vector<Node<FT>*>>(nodes.size());
    for (const auto& node : nodes)
    {
        if (!removed[node.id])
            query_ixns[node.id] = tree.query(node.box);
    }
    auto brute_ixns = std::vector<std::vector<Node<FT>*>>(nodes.size());
    for (const auto& node : nodes)
    {
        if (!removed[node.id])
            brute_ixns[node.id] = brute_query(node.box, nodes, removed);
    }
    for (const auto& node : nodes)
    {
        if (!removed[node.id])
        {
            REQUIRE(equal_intersections(query_ixns[node.id], brute_ixns[node.id]));
        }
    }
}

template <class FT>
void test_insert_remove_find_all(Quadtree<FT> tree, std::vector<Node<FT>>& nodes)
{
    // Randomly remove some nodes
    auto rng = std::default_random_engine();
    auto death_dist = std::uniform_int_distribution(0, 1);
    auto removed = std::vector<bool>(nodes.size());
    std::generate(std::begin(removed), std::end(removed),
        [&rng, &death_dist](){ return death_dist(rng); });
    for (auto& node : nodes)
    {
        if (removed[node.id])
            tree.erase(&node);
    }
    auto query_ixns = tree.find_all_intersections();
    auto brute_ixns = brute_find_all_intersections(nodes, removed);
    REQUIRE(equal_intersections(query_ixns, brute_ixns));
}

#define _run_testcase_FT(func_, FT_, maxsize_)                          \
    do {                                                                \
        using Box = bounding_box<FT_>;                                  \
        using Vec = typename Box::vec_type;                             \
        auto n = get_param();                                           \
        auto box = Box(Vec(0.0f), Vec(100*maxsize_));                   \
        auto nodes = generate_random_nodes<FT_>(n, 100*maxsize_, maxsize_); \
        auto tree = Quadtree<FT_>(box, get_box);                        \
        for (auto& node : nodes)                                        \
            tree.insert(&node);                                         \
        func_<FT_>(std::move(tree), nodes);                             \
    } while(0)

#define _define_testcase(desc_, func_)              \
    TEST_CASE(desc_)                                \
    {                                               \
        SUBCASE("with float") {                     \
            _run_testcase_FT(func_, float, 0.01f);  \
        }                                           \
        SUBCASE("with int") {                       \
            _run_testcase_FT(func_, int, 10);       \
        }                                           \
    }

_define_testcase("quadtree: insert and query", test_insert_and_query)
_define_testcase("quadtree: insert and find all intersections", test_find_all)
_define_testcase("quadtree: insert remove and query", test_insert_remove_query)
_define_testcase("quadtree: insert remove and find all intersections", test_insert_remove_find_all)

}
