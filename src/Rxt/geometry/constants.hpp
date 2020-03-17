#pragma once

#include <CGAL/Origin.h>

namespace Rxt
{
namespace constants
{
template <class K>
constexpr CGAL::Point_3<K> zero {CGAL::ORIGIN};

template <class K>
constexpr CGAL::Vector_3<K> axis[3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
};

template <class K>
constexpr CGAL::Direction_3<K> direction[3] = {
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1}
};
}
}
