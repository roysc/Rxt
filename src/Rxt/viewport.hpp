#pragma once
#include "Rxt/vec.hpp"

#include <type_traits>

namespace Rxt
{
struct _viewport_base {};

template <class Num>
struct basic_viewport : _viewport_base
{
    using float_vec = vec::fvec2;
    using position_type = vec::tvec2<Num>;
    using size_type = vec::tvec2<std::make_unsigned_t<Num>>;

    const size_type m_max_scale;
    const size_type m_min_scale;
    size_type m_scale_factor;
    position_type m_position {0};
    const float margin_size = .1;

    // Construct a viewport with max and min scalable size limits (in cells per window)
    basic_viewport(size_type max, size_type min = size_type(1))
        : m_max_scale(max)
        , m_min_scale(min)
        , m_scale_factor(min)
    {
        // assert(all(lessThan(size_type(0), scale)));
        // assert(all(lessThan(size_type(0), max)));
        // if (!all(lessThanEqual(m_min_scale, m_max_scale)))
        //     throw std::invalid_argument("min scale > max scale");
    }

    virtual void set_position(position_type pos) { m_position = pos; }
    virtual void set_scale(size_type scale) { m_scale_factor = scale; }

    position_type position() const { return m_position; }
    size_type scale_factor() const { return m_scale_factor; }
    size_type max_scale() const { return m_max_scale; }
    size_type size_pixels() const { return m_min_scale * m_max_scale; }

    size_type scale_pow(int exp) const
    {
        if (exp > 0) {
            if (m_scale_factor.x > m_min_scale.x && m_scale_factor.y > m_min_scale.y)
                return m_scale_factor / 2u;
        } else {
            if (m_scale_factor.x < m_max_scale.x && m_scale_factor.y < m_max_scale.y)
                return m_scale_factor * 2u;
        }
        return m_scale_factor;
    }

    // size in number of cells
    size_type size_cells() const
    {
        return size_type(vec::fvec2(size_pixels()) / vec::fvec2(m_scale_factor));
    }

    auto from_nds(float x, float y) const
    {
        return floor(vec::fvec2(x, y) * vec::fvec2(size_cells() / 2u));
    }

    vec::fvec2 to_nds(position_type p) const
    {
        return vec::fvec2(p) / vec::fvec2(size_cells() / 2u);
    }

    vec::fmat4 view_matrix() const
    {
        using vec::fvec3;

        auto pos3 = fvec3(position(), 0);
        vec::fmat4 view_matrix =
            vec::scale(fvec3(2.f / vec::fvec2(size_cells()), 0)) *
            vec::translate(-pos3);
        return view_matrix;
    }

    vec::fmat4 projection_matrix() const
    {
        auto pos = position();
        auto corner = position() + size_cells();
        return vec::ortho(pos.x, corner.x, pos.y, corner.y);
    }

    void translate(position_type d)
    {
        set_position(position() + d);
    }

    // scale, adjusting to maintain position
    void scale_to(size_type coef, position_type focw)
    {
        using vec::fvec2;

        auto coefr = fvec2(coef) / fvec2(m_max_scale);
        position_type newpos = position_type(coefr * fvec2(position() - focw)) + focw;
        set_position(newpos);
        set_scale(coef);
    }

    bool edge_scroll(position_type cursor_position, int speed)
    {
        // (0,0) is center-screen, so offset it to the corner
        auto vpsize = size_cells();
        auto offset_pos = to_nds(cursor_position + position_type(vpsize / 2u));
        position_type dv {0};

        for (unsigned i = 0; i < dv.length(); ++i) {
            if (offset_pos[i] < margin_size) {
                dv[i] = -speed;
                // dv[dv.offset_pos < margin_size] = -speed;
            } else if (offset_pos[i] + margin_size >= 2) {
                dv[i] = +speed;
            }
        }
        if (dv != position_type(0)) {
            translate(dv);
            return true;
        }
        return false;
    }
};
}

template <class V>
struct fmt::formatter<V, std::enable_if_t<std::is_base_of_v<Rxt::_viewport_base, V>, char>>
{
    template <class PC>
    constexpr auto parse(PC& ctx)
    {
        return ctx.begin();
    }

    template <class FC>
    auto format(const V& vp, FC& ctx)
    {
        return fmt::format_to(
            ctx.out(), "viewport(scale={}, max={}, position={})",
            vp.scale_factor(), vp.max_scale(), vp.position()
        );
    }
};
