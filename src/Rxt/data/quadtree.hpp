/*
  MIT License

  Copyright (c) 2019 Pierre Vigier

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

  Source: https://github.com/pvigier/Quadtree
*/
#pragma once

#include "bounding_box.hpp"
#include "Rxt/vec.hpp"

#include <cassert>
#include <algorithm>
#include <memory>
#include <type_traits>
#include <vector>

namespace Rxt
{
template<class T, class GetBox, class FT = float, class Equal = std::equal_to<T>>
struct quadtree
{
    using box_type = Rxt::bounding_box<FT>;

    struct node_type
    {
        std::array<std::unique_ptr<node_type>, 4> children;
        std::vector<T> values;
    };

    static constexpr auto threshold = 16u;
    static constexpr auto max_depth = 8u;

    box_type m_bounds;
    std::unique_ptr<node_type> m_root;
    GetBox m_get_box;
    Equal m_equal;

    static_assert(std::is_convertible_v<std::invoke_result_t<GetBox, const T&>, box_type>,
        "GetBox must be a callable of signature quadtree::box_type(const T&)");
    static_assert(std::is_convertible_v<std::invoke_result_t<Equal, const T&, const T&>, bool>,
        "Equal must be a callable of signature bool(const T&, const T&)");
    static_assert(std::is_arithmetic_v<FT>);

    quadtree(const box_type& box,
             const GetBox& get_box = GetBox{},
             const Equal& equal = Equal{})
        : m_bounds(box)
        , m_root(std::make_unique<node_type>())
        , m_get_box(get_box)
        , m_equal(equal)
    {}

    void insert(const T& value)
    {
        _insert(m_root.get(), 0, m_bounds, value);
    }

    void erase(const T& value)
    {
        _erase(m_root.get(), nullptr, m_bounds, value);
    }

    auto query(const box_type& box) const
    {
        std::vector<T> values;
        _query(m_root.get(), m_bounds, box, values);
        return values;
    }

    auto find_all_intersections() const
    {
        std::vector<std::pair<T, T>> intersections;
        find_all_intersections(m_root.get(), intersections);
        return intersections;
    }

    bool is_leaf(const node_type* node) const
    {
        return !static_cast<bool>(node->children[0]);
    }

    box_type compute_box(const box_type& box, int i) const
    {
        auto origin = box.left();
        auto child_size = box.shape() / static_cast<FT>(2);
        switch (i)
        {
            // North West
            case 0:
                return box_type(origin, child_size);
            // Norst East
            case 1:
                return box_type(vec::tvec2<FT>(origin.x + child_size.x, origin.y), child_size);
            // South West
            case 2:
                return box_type(vec::tvec2<FT>(origin.x, origin.y + child_size.y), child_size);
            // South East
            case 3:
                return box_type(origin + child_size, child_size);
            default:
                assert(false && "Invalid child index");
                return box_type();
        }
    }

    int get_quadrant(const box_type& node_box, const box_type& value_box) const
    {
        auto center = node_box.center();
        auto far = node_box.right();
        // West
        if (far.x < center.x)
        {
            // North West
            if (far.y < center.y)
                return 0;
            // South West
            else if (value_box.left().y >= center.y)
                return 2;
            // Not contained in any quadrant
            else
                return -1;
        }
        // East
        else if (value_box.left().x >= center.x)
        {
            // North East
            if (value_box.right().y < center.y)
                return 1;
            // South East
            else if (value_box.left().y >= center.y)
                return 3;
            // Not contained in any quadrant
            else
                return -1;
        }
        // Not contained in any quadrant
        else
            return -1;
    }

    void _insert(node_type* node, std::size_t depth, const box_type& box, const T& value)
    {
        print("box={} box(value)={}\n", box, m_get_box(value));
        assert(node != nullptr);
        assert(box.contains(m_get_box(value)));
        if (is_leaf(node)) {
            // Insert the value in this node if possible
            if (depth >= max_depth || node->values.size() < threshold)
                node->values.push_back(value);
            // Otherwise, we split and we try again
            else {
                split(node, box);
                _insert(node, depth, box, value);
            }
        } else {
            auto i = get_quadrant(box, m_get_box(value));
            // Add the value in a child if the value is entirely contained in it
            if (i != -1)
                _insert(node->children[static_cast<std::size_t>(i)].get(),
                    depth + 1,
                    compute_box(box, i),
                    value);
            // Otherwise, we add the value in the current node
            else
                node->values.push_back(value);
        }
    }

    void split(node_type* node, const box_type& box)
    {
        assert(node != nullptr);
        assert(is_leaf(node) && "Only leaves can be split");
        // Create children
        for (auto& child : node->children)
            child = std::make_unique<node_type>();
        // Assign values to children
        auto new_values = std::vector<T>(); // New values for this node
        for (const auto& value : node->values) {
            auto i = get_quadrant(box, m_get_box(value));
            if (i != -1)
                node->children[static_cast<std::size_t>(i)]->values.push_back(value);
            else
                new_values.push_back(value);
        }
        node->values = std::move(new_values);
    }

    void _erase(node_type* node, node_type* parent, const box_type& box, const T& value)
    {
        assert(node != nullptr);
        assert(box.contains(m_get_box(value)));
        if (is_leaf(node)) {
            // Remove the value from node
            remove_value(node, value);
            // Try to merge the parent
            if (parent != nullptr)
                try_merge(parent);
        } else {
            // Remove the value in a child if the value is entirely contained in it
            auto i = get_quadrant(box, m_get_box(value));
            if (i != -1)
                _erase(node->children[static_cast<std::size_t>(i)].get(), node, compute_box(box, i), value);
            // Otherwise, we remove the value from the current node
            else
                remove_value(node, value);
        }
    }

    void remove_value(node_type* node, const T& value)
    {
        // Find the value in node->values
        auto it = std::find_if(std::begin(node->values), std::end(node->values),
            [this, &value](const auto& rhs){ return m_equal(value, rhs); });
        assert(it != std::end(node->values) && "Trying to remove a value that is not present in the node");
        // Swap with the last element and pop back
        *it = std::move(node->values.back());
        node->values.pop_back();
    }

    void try_merge(node_type* node)
    {
        assert(node != nullptr);
        assert(!is_leaf(node) && "Only interior nodes can be merged");
        auto nbValues = node->values.size();
        for (const auto& child : node->children) {
            if (!is_leaf(child.get()))
                return;
            nbValues += child->values.size();
        }
        if (nbValues <= threshold) {
            node->values.reserve(nbValues);
            // Merge the values of all the children
            for (const auto& child : node->children) {
                for (const auto& value : child->values)
                    node->values.push_back(value);
            }
            // Remove the children
            for (auto& child : node->children)
                child.reset();
        }
    }

    void _query(node_type* node,
               const box_type& box,
               const box_type& query_box,
               std::vector<T>& values) const
    {
        assert(node != nullptr);
        assert(query_box.intersects(box));
        for (const auto& value : node->values) {
            if (query_box.intersects(m_get_box(value)))
                values.push_back(value);
        }
        if (!is_leaf(node)) {
            for (auto i = std::size_t(0); i < node->children.size(); ++i) {
                auto child_box = compute_box(box, static_cast<int>(i));
                if (query_box.intersects(child_box))
                    _query(node->children[i].get(), child_box, query_box, values);
            }
        }
    }

    void find_all_intersections(node_type* node, std::vector<std::pair<T, T>>& intersections) const
    {
        // Find intersections between values stored in this node
        // Make sure to not report the same intersection twice
        for (auto i = std::size_t(0); i < node->values.size(); ++i)
        {
            for (auto j = std::size_t(0); j < i; ++j)
            {
                if (m_get_box(node->values[i]).intersects(m_get_box(node->values[j])))
                    intersections.emplace_back(node->values[i], node->values[j]);
            }
        }
        if (!is_leaf(node))
        {
            // Values in this node can intersect values in descendants
            for (const auto& child : node->children)
            {
                for (const auto& value : node->values)
                    find_intersections_in_descendants(child.get(), value, intersections);
            }
            // Find intersections in children
            for (const auto& child : node->children)
                find_all_intersections(child.get(), intersections);
        }
    }

    void find_intersections_in_descendants(
        node_type* node,
        const T& value,
        std::vector<std::pair<T, T>>& intersections) const
    {
        // Test against the values stored in this node
        for (const auto& other : node->values)
        {
            if (m_get_box(value).intersects(m_get_box(other)))
                intersections.emplace_back(value, other);
        }
        // Test against values stored into descendants of this node
        if (!is_leaf(node))
        {
            for (const auto& child : node->children)
                find_intersections_in_descendants(child.get(), value, intersections);
        }
    }
};
}
