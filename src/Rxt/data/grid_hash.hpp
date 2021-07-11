#pragma once

#include "bounding_box.hpp"
#include <Rxt/vec.hpp>
#include <Rxt/range.hpp>

#include <unordered_map>
#include <list>
#include <set>
#include <type_traits>
#include <iterator>

namespace Rxt
{
template <class T, class GetBox, class FT = float>
struct grid_hash_map
{
    static constexpr unsigned dimension = 2;

    using mapped_type = T;
    using bucket_type = std::list<mapped_type>;

    using point_type = vec::basic_vec<dimension, FT>;
    using bounds_type = bounding_box<FT>;

    using key_type = vec::basic_vec<dimension, int>;
    using key_interval = bounding_box<int>;

    using Map = std::unordered_map<key_type, bucket_type>;

    point_type m_cell_size;
    GetBox m_get_box;
    Map m_map;

    grid_hash_map(point_type cell_size, GetBox gb = {})
        : m_cell_size(cell_size), m_get_box(gb)
    {}

    // Iterates all cells in [left, right]
    struct cell_iter
    {
        key_type left;
        key_type const diff;
        int index;

        cell_iter(key_type left, key_type right)
            : left(left)
            , diff(right - left + 1)
            , index(vec::product(diff) - 1)
        {}

        auto& operator++() { --index; return *this; }
        key_type operator*() const { return left + vec::unravel_index(index, diff); }
        bool operator!=(range_sentinel) const { return index >= 0; }
    };

    auto _point_cell(point_type point) const
    {
        if constexpr (std::is_integral_v<FT>) {
            return key_type(point) / key_type(m_cell_size);
        } else {
            return point / m_cell_size;
        }
    }

    // map bounding box to grid cell box
    auto _box_cells(bounds_type box) const
    {
        return key_interval(_point_cell(box.left()), _point_cell(box.right()));
    }

    auto cell_range(bounds_type box) const
    {
        auto dest_box = _box_cells(box);
        return range_adaptor<cell_iter, range_sentinel>(
            cell_iter{dest_box.left(), dest_box.right()}, {}
        );
    }

    void insert(mapped_type val)
    {
        for (auto cell: cell_range(m_get_box(val)))
            _insert(cell, val);
    }

    void _insert(key_type cell, mapped_type val)
    {
        auto [it, did] = m_map.emplace(cell, bucket_type());
        it->second.push_back(val);
    }

    // Moves a previously mapped value
    void replace(mapped_type val, bounds_type from)
    {
        auto dest = m_get_box(val);
        auto grid_dest = _box_cells(dest);
        auto from_grid = _box_cells(from);
        // remove, then insert, skipping cells it's still in
        for (auto cell: cell_range(from)) {
            if (grid_dest.contains(cell)) continue;
            _erase(cell, val);
        }
        for (auto cell: cell_range(dest)) {
            if (from_grid.contains(cell)) continue;
            _insert(cell, val);
        }
    }

    void erase(mapped_type val, bounds_type box)
    {
        for (auto cell: cell_range(box))
            _erase(cell, val);
    }

    void erase(mapped_type val)
    {
        erase(val, m_get_box(val));
    }

    void _erase(key_type cell, mapped_type val)
    {
        if (auto it = m_map.find(cell); it != m_map.end()) {
            it->second.remove(val); // todo - any way to use O(1) erase() ?
            if (it->second.empty())
                m_map.erase(it);
        }
    }

    // return all objects intersecting query box
    auto query(bounds_type qbox) const
    {
        std::set<mapped_type> ret;
        for (auto cell: cell_range(qbox)) {
            auto it = m_map.find(cell);
            if (it == m_map.end()) continue;
            for (auto val: it->second) {
                if (qbox.intersects(m_get_box(val)))
                    ret.insert(val);
            }
        }
        return ret;
    }

    // return all objects intersecting a point
    auto query(point_type qpoint) const
    {
        std::set<mapped_type> ret;
        auto it = m_map.find(_point_cell(qpoint));
        if (it != m_map.end()) {
            for (auto val: it->second) {
                if (m_get_box(val).contains(qpoint))
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
                    for (auto jt = std::next(it); jt != last; ++jt) {
                        std::pair<mapped_type, mapped_type> pair{*it, *jt};
                        if (!m_get_box(pair.first).intersects(m_get_box(pair.second)))
                            continue;
                        if (pair.second < pair.first) {
                            using std::swap;
                            swap(pair.first, pair.second);
                        }
                        ret.insert(pair);
                    }
                }
            }
        }
        return ret;
    }
};
}
