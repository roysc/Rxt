#pragma once
#include "camera.hpp"

namespace Rxt
{
namespace frp
{
// second layer of crtp...
template <class Der>
struct focus_cam : _det::focus_cam_crt<focus_cam<Der>>
{
    using super_type = _det::focus_cam_crt<focus_cam<Der>>;
    using super_type::super_type;

    void set_position(vec3 pos)
    {
        super_type::set_position(pos);
        static_cast<Der&>(*this).on_update();
    }
};
}
}
