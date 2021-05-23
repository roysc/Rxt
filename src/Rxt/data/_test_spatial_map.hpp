#pragma once

#include "bounding_box.hpp"

#include <vector>
#include <random>
#include <utility>
#include <algorithm>
#include <type_traits>
#include <iterator>

namespace Rxt
{
template <class FT>
struct Node
{
    using Box = bounding_box<FT>;
    using Vec = typename Box::vec_type;
    Box box;
    std::size_t id;
};

using std::vector;
using std::pair;

template <class FT>
auto generate_random_nodes(std::size_t n, FT maxpos, FT maxsize)
{
    using Dist = std::conditional_t<
        std::is_integral_v<FT>,
        std::uniform_int_distribution<FT>,
        std::uniform_real_distribution<FT>
    >;

    std::default_random_engine rng;
    Dist origin_distribution(0, maxpos);
    Dist size_distribution(0, maxsize);
    vector<Node<FT>> nodes(n);
    for (auto i = 0u; i < n; ++i) {
        using Vec = Node<FT>::Vec;
        using Box = Node<FT>::Box;
        auto mincorner = Vec(origin_distribution(rng), origin_distribution(rng));
        nodes[i].box = Box(
            mincorner,
            min(Vec(maxpos), mincorner + Vec(size_distribution(rng), size_distribution(rng)))
        );
        nodes[i].id = i;
    }
    return nodes;
}

// brute-force query
template <class FT>
auto brute_query(const bounding_box<FT>& box, vector<Node<FT>>& nodes, const vector<bool>& removed)
{
    auto intersections = vector<Node<FT>*>();
    for (auto& n : nodes) {
        if (removed.size() == 0 || !removed[n.id]) {
            if (box.intersects(n.box))
                intersections.push_back(&n);
        }
    }
    return intersections;
}

template <class FT>
auto brute_find_all_intersections(vector<Node<FT>>& nodes, const vector<bool>& removed)
{
    auto intersections = vector<pair<Node<FT>*, Node<FT>*>>();
    for (auto i = std::size_t(0); i < nodes.size(); ++i) {
        if (removed.size() == 0 || !removed[i]) {
            for (auto j = std::size_t(0); j < i; ++j) {
                if (removed.size() == 0 || !removed[j]) {
                    if (nodes[i].box.intersects(nodes[j].box))
                        intersections.emplace_back(&nodes[i], &nodes[j]);
                }
            }
        }
    }
    return intersections;
}

template <class FT>
bool equal_intersections(vector<Node<FT>*> nodes1, vector<Node<FT>*> nodes2)
{
    if (nodes1.size() != nodes2.size())
        return false;
    std::sort(std::begin(nodes1), std::end(nodes1));
    std::sort(std::begin(nodes2), std::end(nodes2));
    return nodes1 == nodes2;
}

template <class FT>
bool equal_intersections(
    vector<pair<Node<FT>*, Node<FT>*>> intersections1,
    vector<pair<Node<FT>*, Node<FT>*>> intersections2)
{
    if (intersections1.size() != intersections2.size())
        return false;
    for (auto& intersection : intersections1) {
        if (intersection.first >= intersection.second)
            std::swap(intersection.first, intersection.second);
    }
    for (auto& intersection : intersections2) {
        if (intersection.first >= intersection.second)
            std::swap(intersection.first, intersection.second);
    }
    std::sort(std::begin(intersections1), std::end(intersections1));
    std::sort(std::begin(intersections2), std::end(intersections2));
    return intersections1 == intersections2;
}

template <class Src, class Dst>
void copy_range(Src const& src, Dst& dst)
{
    std::copy(src.begin(), src.end(), std::back_inserter(dst));
}

// Test case templates

template <class FT, class SpatialMap>
void test_insert_and_query(SpatialMap space, vector<Node<FT>>& nodes)
{
    auto query_ixns = vector<vector<Node<FT>*>>(nodes.size());
    for (const auto& node : nodes)
        copy_range(space.query(node.box), query_ixns[node.id]);
    auto brute_ixns = vector<vector<Node<FT>*>>(nodes.size());
    for (const auto& node : nodes)
        brute_ixns[node.id] = brute_query(node.box, nodes, {});
    for (const auto& node : nodes)
        REQUIRE(equal_intersections(query_ixns[node.id], brute_ixns[node.id]));
}

template <class FT, class SpatialMap>
void test_find_all(SpatialMap space, vector<Node<FT>>& nodes)
{
    vector<pair<Node<FT>*, Node<FT>*>> query_ixns;
    copy_range(space.find_all_intersections(), query_ixns);
    auto brute_ixns = brute_find_all_intersections(nodes, {});
    REQUIRE(equal_intersections(query_ixns, brute_ixns));
}

template <class FT, class SpatialMap>
void test_insert_remove_query(SpatialMap space, vector<Node<FT>>& nodes)
{
    // Randomly remove some nodes
    auto rng = std::default_random_engine();
    auto death_dist = std::uniform_int_distribution(0, 1);
    auto removed = vector<bool>(nodes.size());
    std::generate(std::begin(removed), std::end(removed),
        [&rng, &death_dist](){ return death_dist(rng); }
    );
    for (auto& node : nodes) {
        if (removed[node.id])
            space.erase(&node);
    }
    auto query_ixns = vector<vector<Node<FT>*>>(nodes.size());
    for (const auto& node : nodes) {
        if (!removed[node.id])
            copy_range(space.query(node.box), query_ixns[node.id]);
    }
    auto brute_ixns = vector<vector<Node<FT>*>>(nodes.size());
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

template <class FT, class SpatialMap>
void test_insert_remove_find_all(SpatialMap space, vector<Node<FT>>& nodes)
{
    // Randomly remove some nodes
    auto rng = std::default_random_engine();
    auto death_dist = std::uniform_int_distribution(0, 1);
    auto removed = vector<bool>(nodes.size());
    std::generate(std::begin(removed), std::end(removed),
        [&rng, &death_dist](){ return death_dist(rng); });
    for (auto& node : nodes)
    {
        if (removed[node.id])
            space.erase(&node);
    }
    vector<pair<Node<FT>*, Node<FT>*>> query_ixns;
    copy_range(space.find_all_intersections(), query_ixns);
    auto brute_ixns = brute_find_all_intersections(nodes, removed);
    REQUIRE(equal_intersections(query_ixns, brute_ixns));
}

}
