#pragma once

#include "viewport.hpp"
#include "reactive.hpp"
#include "vec.hpp"
#include <Rxt/graphics/camera.hpp>

// FRP-style controls for 2-D input

namespace Rxt
{
template <class FT>
struct basic_cursor
{
    using position_type = vec::tvec2<FT>;
    position_type m_position{0};

    position_type position() const { return m_position; }
    void set_position(position_type pos) { this->m_position = pos; }
};

template <class Der, class FT>
struct reactive_cursor : basic_cursor<FT>, reactive_base<Der>
{
    using super_type = basic_cursor<FT>;
    using super_type::super_type;
    using super_type::position;
    using position_type = FT;

    void set_position(position_type pos) { super_type::set_position(pos); this->do_update(); }
};

template <class Der, class FT>
struct reactive_viewport : basic_viewport<FT>, reactive_base<Der>
{
    using super_type = basic_viewport<FT>;
    using super_type::super_type;
    using super_type::position;

    using position_type = typename super_type::position_type;
    using size_type = typename super_type::size_type;

    void set_position(position_type p) override { super_type::set_position(p); this->do_update(); }
    void set_scale(size_type s) override { super_type::set_scale(s); this->do_update(); }
};

template <class Der, class Cam = Rxt::focused_camera>
struct reactive_camera : Cam, reactive_base<Der>
{
    using super_type = Cam;
    using super_type::super_type;
    using super_type::position;
    reactive_camera(super_type c) : super_type(c) {}

    void position(super_type::position_type pos) override
    {
        super_type::position(pos);
        this->do_update();
    }
};
}
