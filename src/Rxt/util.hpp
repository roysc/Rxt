#pragma once

#include "Rxt/vec.hpp"

#include <utility>
#include <functional>

namespace Rxt
{
// sort the corners and return canonical form
template <class T>
std::pair<T, T> box(T a, T b)
{
    auto a_ = min(a, b);
    auto b_ = max(a, b);
    return std::make_pair(a_, b_);
}

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

auto get_first = [] (auto&& pair) -> auto&& { return (pair.first); };
auto get_second = [] (auto&& pair) -> auto&& { return (pair.second); };

// "Lazy" function wrapper to batch calls and flush manually
struct lazy_action
{
    using action_function = std::function<void()>;

    action_function function;
    unsigned count = 0;

    template <class F>
    lazy_action(F&& f) : function{f} {}

    void operator()() { ++count; }

    unsigned flush()
    {
        if (count && function) {
            function();
            auto ret = count;
            count = 0;
            return ret;
        }
        return 0;
    }
};

struct runtime //todo nix
{
    bool _running = true;

    bool is_stopped() const { return !_running; }
    void is_stopped(bool v) { _running = !v; }

    virtual ~runtime() {}
};

using simple_runtime = runtime;

template <class FT>
struct bounding_box
{
    using vec_type = vec::tvec2<FT>;
    vec_type _min, _max;

    bounding_box(vec_type a, vec_type b)
        : _min(min(a, b))
        , _max(max(a,b ))
    {}

    bounding_box(std::pair<vec_type, vec_type> pair)
        : bounding_box(pair.first, pair.second) {}

    bounding_box() : bounding_box(vec_type(), vec_type()) {}

    auto shape() const { return _max - _min; }
    auto center() const { return _min + shape() / FT(2); }

    auto left() const { return _min; }
    auto right() const { return _max; }

    bool contains(const bounding_box& box) const
    {
        return all(lessThanEqual(left(), box.left())) && all(lessThanEqual(box.right(), right()));
    }

    bool intersects(const bounding_box& box) const
    {
        return !any(greaterThanEqual(left(), box.right()) || greaterThanEqual(box.left(), right()));
    }
};

template <class FT>
bounding_box(vec::tvec2<FT>, vec::tvec2<FT>) -> bounding_box<FT>;

template <class T>
auto make_box(T a, T b)
{
    return bounding_box(a, b);
}
}
