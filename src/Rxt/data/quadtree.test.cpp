#include "quadtree.hpp"
#include "Rxt/_testing.hpp"

#include <vector>
#include <random>
#include <utility>
#include <algorithm>

namespace Rxt
{
inline int GetParam() { return 100; }

using Box = bounding_box<float>;
using Vec = typename Box::vec_type;
struct Node
{
    Box box;
    std::size_t id;
};

std::vector<Node> generateRandomNodes(std::size_t n)
{
    auto generator = std::default_random_engine();
    auto originDistribution = std::uniform_real_distribution(0.0f, 1.0f);
    auto sizeDistribution = std::uniform_real_distribution(0.0f, 0.01f);
    auto nodes = std::vector<Node>(n);
    for (auto i = std::size_t(0); i < n; ++i)
    {
        auto mincorner = Vec(originDistribution(generator), originDistribution(generator));
        nodes[i].box = Box(
            mincorner,
            min(1.0f - mincorner, Vec(sizeDistribution(generator), sizeDistribution(generator)))
        );
        nodes[i].id = i;
    }
    return nodes;
}

std::vector<Node*> query(const Box& box, std::vector<Node>& nodes, const std::vector<bool>& removed)
{
    auto intersections = std::vector<Node*>();
    for (auto& n : nodes)
    {
        if (removed.size() == 0 || !removed[n.id])
        {
            if (box.intersects(n.box))
                intersections.push_back(&n);
        }
    }
    return intersections;
}

std::vector<std::pair<Node*, Node*>> findAllIntersections(std::vector<Node>& nodes, const std::vector<bool>& removed)
{
    auto intersections = std::vector<std::pair<Node*, Node*>>();
    for (auto i = std::size_t(0); i < nodes.size(); ++i)
    {
        if (removed.size() == 0 || !removed[i])
        {
            for (auto j = std::size_t(0); j < i; ++j)
            {
                if (removed.size() == 0 || !removed[j])
                {
                    if (nodes[i].box.intersects(nodes[j].box))
                        intersections.emplace_back(&nodes[i], &nodes[j]);
                }
            }
        }
    }
    return intersections;
}

bool checkIntersections(std::vector<Node*> nodes1, std::vector<Node*> nodes2)
{
    if (nodes1.size() != nodes2.size())
        return false;
    std::sort(std::begin(nodes1), std::end(nodes1));
    std::sort(std::begin(nodes2), std::end(nodes2));
    return nodes1 == nodes2;
}

bool checkIntersections(std::vector<std::pair<Node*, Node*>> intersections1,
    std::vector<std::pair<Node*, Node*>> intersections2)
{
    if (intersections1.size() != intersections2.size())
        return false;
    for (auto& intersection : intersections1)
    {
        if (intersection.first >= intersection.second)
            std::swap(intersection.first, intersection.second);
    }
    for (auto& intersection : intersections2)
    {
        if (intersection.first >= intersection.second)
            std::swap(intersection.first, intersection.second);
    }
    std::sort(std::begin(intersections1), std::end(intersections1));
    std::sort(std::begin(intersections2), std::end(intersections2));
    return intersections1 == intersections2;
}

TEST_CASE("quadtree: add and query")
{
    auto n = GetParam();
    auto getBox = [](Node* node)
    {
        return node->box;
    };
    auto box = Box(Vec(0.0f), Vec(1.0f));
    auto nodes = generateRandomNodes(n);
    // Add nodes to quadtree
    auto tree = quadtree<Node*, decltype(getBox)>(box, getBox);
    for (auto& node : nodes)
        tree.add(&node);
    // Quadtree
    auto intersections1 = std::vector<std::vector<Node*>>(nodes.size());
    for (const auto& node : nodes)
        intersections1[node.id] = tree.query(node.box);
    // Brute force
    auto intersections2 = std::vector<std::vector<Node*>>(nodes.size());
    for (const auto& node : nodes)
        intersections2[node.id] = query(node.box, nodes, {});
    // Check
    for (const auto& node : nodes)
        REQUIRE(checkIntersections(intersections1[node.id], intersections2[node.id]));
}

TEST_CASE("quadtree: add and find all intersections")
{
    auto n = GetParam();
    auto getBox = [](Node* node)
    {
        return node->box;
    };
    auto box = Box(Vec(0.0f), Vec(1.0f));
    auto nodes = generateRandomNodes(n);
    // Add nodes to quadtree
    auto quadtree = Rxt_ds::quadtree<Node*, decltype(getBox)>(box, getBox);
    for (auto& node : nodes)
        quadtree.add(&node);
    // Quadtree
    auto intersections1 = quadtree.find_all_intersections();
    // Brute force
    auto intersections2 = findAllIntersections(nodes, {});
    // Check
    REQUIRE(checkIntersections(intersections1, intersections2));
}

TEST_CASE("quadtree: add remove and query")
{
    auto n = GetParam();
    auto getBox = [](Node* node)
    {
        return node->box;
    };
    auto box = Box(Vec(0.0f), Vec(1.0f));
    auto nodes = generateRandomNodes(n);
    // Add nodes to quadtree
    auto quadtree = Rxt_ds::quadtree<Node*, decltype(getBox)>(box, getBox);
    for (auto& node : nodes)
        quadtree.add(&node);
    // Randomly remove some nodes
    auto generator = std::default_random_engine();
    auto deathDistribution = std::uniform_int_distribution(0, 1);
    auto removed = std::vector<bool>(nodes.size());
    std::generate(std::begin(removed), std::end(removed),
        [&generator, &deathDistribution](){ return deathDistribution(generator); });
    for (auto& node : nodes)
    {
        if (removed[node.id])
            quadtree.remove(&node);
    }
    // Quadtree
    auto intersections1 = std::vector<std::vector<Node*>>(n);
    for (const auto& node : nodes)
    {
        if (!removed[node.id])
            intersections1[node.id] = quadtree.query(node.box);
    }
    // Brute force
    auto intersections2 = std::vector<std::vector<Node*>>(n);
    for (const auto& node : nodes)
    {
        if (!removed[node.id])
            intersections2[node.id] = query(node.box, nodes, removed);
    }
    // Check
    for (const auto& node : nodes)
    {
        if (!removed[node.id])
        {
            REQUIRE(checkIntersections(intersections1[node.id], intersections2[node.id]));
        }
    }
}

TEST_CASE("quadtree: add remove and find all intersections")
{
    auto n = GetParam();
    auto getBox = [](Node* node)
    {
        return node->box;
    };
    auto box = Box(Vec(0.0f), Vec(1.0f));
    auto nodes = generateRandomNodes(n);
    // Add nodes to quadtree
    auto quadtree = Rxt_ds::quadtree<Node*, decltype(getBox)>(box, getBox);
    for (auto& node : nodes)
        quadtree.add(&node);
    // Randomly remove some nodes
    auto generator = std::default_random_engine();
    auto deathDistribution = std::uniform_int_distribution(0, 1);
    auto removed = std::vector<bool>(nodes.size());
    std::generate(std::begin(removed), std::end(removed),
        [&generator, &deathDistribution](){ return deathDistribution(generator); });
    for (auto& node : nodes)
    {
        if (removed[node.id])
            quadtree.remove(&node);
    }
    // Quadtree
    auto intersections1 = quadtree.find_all_intersections();
    // Brute force
    auto intersections2 = findAllIntersections(nodes, removed);
    // Check
    REQUIRE(checkIntersections(intersections1, intersections2));
}
}
