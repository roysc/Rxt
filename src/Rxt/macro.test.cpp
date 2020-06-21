#include "macro.hpp"
#include "_testing.hpp"

#include <type_traits>
#include <functional>

TEST_CASE("RXT_def_ctors_move_only")
{
    struct MO
    {
        RXT_def_ctors_move_only(MO);
    };

    static_assert(std::is_move_constructible_v<MO>);
    static_assert(!std::is_copy_constructible_v<MO>);
    static_assert(std::is_move_assignable_v<MO>);
    static_assert(!std::is_copy_assignable_v<MO>);
}

TEST_CASE("RXT_set_lambda")
{
    using pv = void(*)();
    using pvi = void(*)(int);
    using fv = std::function<void()>;
    using fvi = std::function<void(int)>;
    static int s_val;
    pv p;
    pvi pi;
    RXT_set_lambda_nocap(p,) {};
    RXT_set_lambda_nocap(pi, int x) { s_val = x; };
    pi(4);
    REQUIRE(s_val == 4);

    fv f;
    fvi fi;
    int val;

    RXT_set_lambda(f,) {};
    RXT_set_lambda(fi, int x) { val = x; };
    fi(4);
    REQUIRE(val == 4);
}
