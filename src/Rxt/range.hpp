#pragma once
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

}
