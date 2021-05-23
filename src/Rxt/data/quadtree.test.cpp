#include "quadtree.hpp"
#include "Rxt/_testing.hpp"
#include "_test_spatial_map.hpp"

#include <utility>

namespace Rxt
{
inline int get_param() { return 100; }
auto get_box = [](auto* node) { return node->box; };

template <class FT>
using Quadtree = quadtree<Node<FT>*, decltype(get_box), FT>;

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
