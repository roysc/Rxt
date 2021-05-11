#pragma once

#include "viewport.hpp"
#include "reactive.hpp"
#include "vec.hpp"
#include <Rxt/camera.hpp>

// FRP-style controls for 2-D input

namespace Rxt
{
template <class Num>
struct basic_cursor
{
    using position_type = vec::tvec2<Num>;
    position_type m_position{};

    position_type position() const { return m_position; }
    void set_position(position_type pos) { this->m_position = pos; }
};

template <class Der, class Num>
struct reactive_cursor : basic_cursor<Num>, reactive_base<Der>
{
    using super_type = basic_cursor<Num>;
    using super_type::super_type;
    using super_type::position;
    using position_type = super_type::position_type;

    reactive_cursor(super_type s) : super_type(s) {}
    void set_position(position_type pos) { super_type::set_position(pos); this->do_update(); }
};

template <class Der, class Num>
struct reactive_viewport : basic_viewport<Num>, reactive_base<Der>
{
    using super_type = basic_viewport<Num>;
    using super_type::super_type;
    using super_type::position;

    using position_type = typename super_type::position_type;
    using size_type = typename super_type::size_type;

    reactive_viewport(super_type s) : super_type(s) {}
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

    void set_position(super_type::position_type pos) override
    {
        super_type::set_position(pos);
        this->do_update();
    }
};
}
