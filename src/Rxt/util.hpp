#pragma once

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

struct runtime
{
    bool _running = true;

    bool should_quit() const { return !_running; }
    void should_quit(bool v) { _running = !v; }

    virtual ~runtime() {}
};

using simple_runtime = runtime;

template <class R>
bool exited(R const& rt) { return rt.should_quit(); }
}
