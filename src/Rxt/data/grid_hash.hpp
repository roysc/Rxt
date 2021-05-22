#pragma once

#include "bounding_box.hpp"
#include <Rxt/vec.hpp>
#include <Rxt/range.hpp>

#include <unordered_map>
#include <list>
#include <set>
#include <type_traits>

namespace Rxt
{
template <class T, class GetBox, class FT = float>
struct grid_hash_map
{
    static constexpr unsigned dimension = 2;
    static_assert(!std::is_integral_v<FT> || std::is_unsigned_v<FT>,
                  "integral field type must be unsigned");

    using mapped_type = T;
    using bucket_type = std::list<mapped_type>;
    using box_type = bounding_box<FT>;

    using key_type = vec::basic_vec<dimension, unsigned>;
    using cell_size_type = vec::basic_vec<dimension, FT>;
    using grid_box_type = bounding_box<unsigned>;

    using Map = std::unordered_map<key_type, bucket_type>;

    // struct iterator
    // {
    //     Map::iterator m_map_iter;
    //     bucket_type::iterator m_bucket_iter;
    // };

    cell_size_type m_cell_size;
    GetBox m_get_box;
    Map m_map;

    grid_hash_map(key_type cell_size, GetBox gb = {})
        : m_cell_size(cell_size), m_get_box(gb)
    {}

    // iterate all cells in [left, right]
    struct CellRangeIter
    {
        // key_type curr, last;
        // unsigned axis = 0;
        key_type left, right;
        key_type diff = right - left + 1u;
        int index = vec::product(diff) - 1;

        auto& operator++()
        {
            // print("index={}, diff={} curr={}\n", index, diff, **this);
            --index;
            return *this;
        }
        key_type operator*() const { return left + vec::unravel_index(index, diff); }
        bool operator!=(range_sentinel) const { return index >= 0; }
    };

    // map bounding box to grid cell box
    auto _grid_box(box_type box) const
    {
        if constexpr (std::is_integral_v<FT>) {
            return grid_box_type(
                key_type(box.left()) / m_cell_size,
                key_type(box.right()) / m_cell_size
            );
        } else { 
            return grid_box_type(
                box.left() / m_cell_size,
                box.right() / m_cell_size
            );
        }
    }

    auto cell_range(box_type box) const
    {
        auto dest_box = _grid_box(box);
        return range_adaptor<CellRangeIter, range_sentinel>(
            CellRangeIter{dest_box.left(), dest_box.right()}, {}
        );
    }

    void insert(mapped_type val)
    {
        for (auto cell: cell_range(m_get_box(val))) {
            auto [it, did] = m_map.emplace(cell, bucket_type());
            it->second.push_back(val);
        }
    }

    // move a previously mapped value
    void replace(mapped_type val, box_type from)
    {
        auto dest = m_get_box(val);
        auto grid_dest = _grid_box(dest);
        for (auto cell: cell_range(from)) {
            if (grid_dest.contains(cell)) continue;
            m_map[cell].erase(val);
        }
        auto from_grid = _grid_box(from);
        for (auto cell: cell_range(dest)) {
            if (from_grid.containts(cell)) continue;
            auto [it, did] = m_map.emplace(cell, bucket_type());
            it->second.push_back(val);
        }
    }

    // void erase(iterator it)
    // it.m_map_iter->second.erase(it);
    void erase(mapped_type val, box_type box)
    {
        for (auto cell: cell_range(box)) {
            m_map[cell].erase(val);
        }
    }

    void erase(mapped_type val)
    {
        erase(val, m_get_box(val));
    }

    // return all objects intersecting query box
    auto query(box_type qbox) const
    {
        std::set<mapped_type> ret;
        // key_type
        //     left(vec::ivec2(qbox.left()) / m_cell_size),
        //     right(vec::ivec2(qbox.right()) / m_cell_size);

        // // scan all intersected cells
        // key_type adj = left;
        // for (unsigned d = 0; d < dimension; ++d) {
        //     do {
        //         for (auto val: m_map.at(adj)) {
        //             if (qbox.intersects(m_get_box(val)))
        //                 ret.insert(val);
        //         }
        //         ++adj[d];
        //     } while (adj[d] < right[d]);
        // }

        for (auto cell: cell_range(qbox)) {
            for (auto val: m_map.at(cell)) {
                if (qbox.intersects(m_get_box(val)))
                    ret.insert(val);
            }
        }
        return ret;
    }

    auto find_all_intersections() const
    {
        std::set<std::pair<mapped_type, mapped_type>> ret;
        for (auto& [key, list]: m_map) {
            if (list.size() >= 2) {
                for (auto it = list.begin(), last = list.end(); it != last; ++it) {
                    for (auto jt = it; jt != last; ++jt) {
                        std::pair<mapped_type, mapped_type> pair{*it, *jt};
                        if (!m_get_box(pair.first).intersects(m_get_box(pair.second)))
                            continue;
                        if (pair.second < pair.first) {
                            using std::swap;
                            swap(pair.first, pair.second);
                        }
                        ret.insert(ret);
                    }
                }
            }
        }
        return ret;
    }
};
}
