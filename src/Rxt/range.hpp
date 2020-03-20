#pragma once

#include "util.hpp"

#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/iterator_range.hpp>

#include <map>
#include <utility>
#include <optional>

namespace Rxt
{
template <class I>
struct range_adaptor
{
    I first, second;
    I begin() const { return this->first; }
    I end() const { return this->second; }
};

// iterator pair to range
template <class I>
auto to_range(std::pair<I, I> p)
{
    return range_adaptor<I> {p.first, p.second};
}

template <class T>
auto to_range(std::optional<T>& p)
{
    auto* ptr = &*p;
    return range_adaptor<T*> {ptr, (p ? ptr + 1 : ptr)};
}

// Given a map M (k -> v) with iterator Mi, output pairs (k, r) for all equal-ranges r:(Mi, Mi) in M
// TODO unit test
template <class Map, class OutIt>
void index_ranges(Map const& m, OutIt out)
{
    for (auto it = m.begin(); it != m.end(); ) {
        auto r = m.equal_range(it->first);

        *out++ = std::make_pair(it->first, r);
        it = r.second;
    }
}

// Given map M (k -> v), return a map of (k -> R v) of keys to ranges of equivalent values
template <class Map>
auto make_indexed_range_map(Map const& m)
{
    using K = typename Map::key_type;
    using It = typename Map::const_iterator;
    using GetSecond = decltype(get_second);
    using Range = boost::iterator_range<boost::transform_iterator<GetSecond, It>>;
    std::map<K, Range> ret;     // todo: use output iterator

    for (auto it = m.begin(); it != m.end(); ) {
        auto r = m.equal_range(it->first);
        Range rr = boost::make_iterator_range(
            boost::make_transform_iterator(r.first, get_second),
            boost::make_transform_iterator(r.second, get_second)
        );
        ret.emplace(it->first, rr);
        it = r.second;
    }
    return ret;
}
}
