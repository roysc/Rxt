#include "grid_hash.hpp"
#include "_test_spatial_map.hpp"

#include <utility>

namespace Rxt
{
inline int get_param() { return 100; }
auto get_box = [](auto* node) { return node->box; };

#define _run_testcase_FT(func_, FT_, cellsize_)                         \
    do {                                                                \
        using Map = grid_hash_map<Node<FT_>*, decltype(get_box), FT_>;  \
        auto n = get_param();                                           \
        auto nodes = generate_random_nodes<FT_>(n, 100*cellsize_, 2*cellsize_); \
        auto map = Map(Map::point_type(cellsize_), get_box);            \
        for (auto& node : nodes)                                        \
            map.insert(&node);                                          \
        func_<FT_>(std::move(map), nodes);                              \
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
        SUBCASE("with uint") {                      \
            _run_testcase_FT(func_, unsigned, 10);  \
        }                                           \
    }

_define_testcase("grid hash: insert and query", test_insert_and_query)
_define_testcase("grid hash: insert and find all intersections", test_find_all)
_define_testcase("grid hash: insert remove and query", test_insert_remove_query)
_define_testcase("grid hash: insert remove and find all intersections", test_insert_remove_find_all)
// todo: replace
}
