#pragma once
#include <type_traits>

namespace Rxt
{
inline namespace meta
{
struct empty {};
struct swallow { template <class... Ts> swallow(Ts&&...) {} };
struct default_ {};

template <class... T>
struct type_tuple {};

using empty_tuple = type_tuple<>;

// indexing
template <class... >
struct index_of;

template <class T, class... R>
struct index_of<T, T, R...>
    : std::integral_constant<std::size_t, 0>
{ };

template <class T, class F, class... R>
struct index_of<T, F, R...>
    : std::integral_constant<std::size_t, 1 + index_of<T, R...>::value>
{ };

template <class... T>
constexpr std::size_t index_of_v = index_of<T...>::value;

template <class N, class Tup>
struct tuple_index_of;

template <class N, class... Ts>
struct tuple_index_of<N, type_tuple<Ts...>>
    : index_of<N, Ts...>
{};

template <class N, class T>
constexpr std::size_t tuple_index_of_v = tuple_index_of<N, T>::value;

// concat
template <class, class>
struct tuple_concat;

template <class... A, class... B>
struct tuple_concat<type_tuple<A...>, type_tuple<B...>> {
    using type = type_tuple<A..., B...>;
};

template <class A, class B>
using tuple_concat_t = typename tuple_concat<A, B>::type;

// transform
template <template <class...> class, class>
struct tuple_apply;

template <template <class...> class F, class... Ts>
struct tuple_apply<F, Rxt::type_tuple<Ts...>>
{
    using type = F<Ts...>;
};

template <template <class...> class F, class TT>
using tuple_apply_t = typename tuple_apply<F, TT>::type;

// map
template <template <class> class, class>
struct tuple_map;

template <template <class> class F, class... Ts>
struct tuple_map<F, Rxt::type_tuple<Ts...>>
{
    using type = type_tuple<F<Ts>...>;
};

template <template <class> class F, class TT>
using tuple_map_t = typename tuple_map<F, TT>::type;
}
}
