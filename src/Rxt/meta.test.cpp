#include "meta.hpp"
#include "_testing.hpp"
#include <type_traits>

namespace Rxt
{
TEST_CASE("index_of")
{
    static_assert(index_of<int, int, char>::value == 0);

    using tt = type_tuple<int, char>;
    static_assert(tuple_index_of_v<int, tt> == 0);
    static_assert(tuple_index_of_v<char, tt> == 1);
}

TEST_CASE("tuple_concat")
{
    using tt1 = type_tuple<int, char>;
    using tt2 = type_tuple<int*>;
    using tt3 = type_tuple<int, char, int*>;

    using ttcat = tuple_concat_t<tt1, tt2>;
    static_assert(std::is_same_v<ttcat, tt3>);
}

template <class... T> struct S {};
TEST_CASE("tuple_apply")
{
    using tt = type_tuple<int, char>;
    using tts = tuple_apply_t<S, tt>;
    static_assert(std::is_same_v<tts, S<int, char>>);
}

template <class T> struct F {};
TEST_CASE("tuple_map")
{
    using tt = type_tuple<int, char>;
    using tts = tuple_map_t<F, tt>;
    static_assert(std::is_same_v<tts, type_tuple<F<int>, F<char>>>);
}
}
