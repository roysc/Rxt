#pragma once

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <vector>

template <class R1, class R2>
bool range_equal(R1 const& r1, R2 const& r2)
{
    bool ret = true;
    auto vit = r2.begin();
    for (auto&& val: r1) {
        if (vit == r2.end()) return false;
        ret &= (val == *vit++);
    }
    return ret && (vit == r2.end());
}
