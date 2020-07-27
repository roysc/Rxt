#include "range.hpp"
#include "Rxt/_testing.hpp"

#include <map>
#include <utility>
#include <iterator>
// #include <experimental/ranges/algorithm>
#include <optional>

namespace Rxt
{
TEST_CASE("to_range")
{
    std::optional<int> xo;
    std::optional<int> const cxo = 8;

    auto count = [] (auto&& r) {
        int ct = 0;
        for (int _: r) ++ct;
        return ct;
    };
    REQUIRE_EQ(count(to_range(xo)), 0);
    xo = 4;
    REQUIRE_EQ(count(to_range(xo)), 1);

    REQUIRE_EQ(count(to_range(cxo)), 1);
}

TEST_CASE("index_ranges(map)")
{
    using M = std::map<int, int>;
    using MI = M::const_iterator;
    using MIP = std::pair<MI, MI>;

    M m = { {1, 10}, {2, 20} };
    std::map<int, MIP> mp;
    index_ranges(m, std::inserter(mp, mp.end()));

    REQUIRE_EQ(mp[1], MIP{begin(m), next(begin(m))});
    REQUIRE_EQ(mp[2], MIP{next(begin(m)), end(m)});
}

TEST_CASE("index_ranges(multimap)")
{
    using M = std::multimap<int, int>;
    using MI = M::const_iterator;
    using MIP = std::pair<MI, MI>;

    M m = { {1, 10}, {2, 20}, {1, 30} };
    std::map<int, MIP> mp;
    index_ranges(m, std::inserter(mp, mp.end()));

    REQUIRE_EQ(mp[1], MIP{begin(m), next(begin(m), 2)});
    REQUIRE_EQ(mp[2], MIP{next(begin(m), 2), end(m)});
}

template <class R>
bool equivalent(R r, std::vector<int> vec)
{
    // using std::experimental::ranges::copy;
    // std::vector<int> rvec;
    // copy(r, rvec);
    // return rvec == vec;
    auto vit = vec.begin();
    bool ret = true;
    for (auto&& val: r) {
        if (vit == vec.end()) return false;
        ret &= (val == *vit++);
    }
    return ret && (vit == vec.end());
}

#ifndef RXT_DISABLE_RANGE_INDEXING
TEST_CASE("make_indexed_range_map(map)")
{
    using M = std::map<int, int>;

    M m = { {1, 10}, {2, 20} };

    auto mr = make_indexed_range_map(m);

    REQUIRE(equivalent(mr[1], {10}));
    REQUIRE(equivalent(mr[2], {20}));
}

TEST_CASE("make_indexed_range_map(multimap)")
{
    using M = std::multimap<int, int>;

    M m = {{1, 10}, {2, 20}, {1, 30}};

    auto mr = make_indexed_range_map(m);

    REQUIRE(equivalent(mr[1], {10, 30}));
    REQUIRE(equivalent(mr[2], {20}));
}
#endif

}
