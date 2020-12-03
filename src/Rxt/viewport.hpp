#pragma once
#include "Rxt/vec.hpp"

#include <type_traits>

namespace Rxt
{
template <class FT>
struct basic_viewport
{
    using float_vec = vec::fvec2;
    using position_type = vec::tvec2<FT>;
    using size_type = vec::tvec2<std::make_unsigned_t<FT>>;

    size_type max_scale;
    size_type scale_factor {1};
    position_type _position {0};
    const size_type size_px {max_scale * scale_factor};
    const float margin_size = .1;

    basic_viewport(size_type max, size_type scale = size_type(1))
        : max_scale{max}, scale_factor{scale}
    {
        assert(all(lessThan(size_type(0), scale)));
        assert(all(lessThan(size_type(0), max)));
    }

    virtual void set_position(position_type pos) { _position = pos; }
    virtual void set_scale(size_type scale) { scale_factor = scale; }

    position_type position() const { return _position; }

    size_type scale_relative(float factor) const
    {
        return scale_factor * factor;
    }

    size_type scale_pow(int exp) const
    {
        const size_type min_scale{1};
        if (exp > 0) {
            if (scale_factor.x > min_scale.x && scale_factor.y > min_scale.y)
                return scale_factor / 2u;
        } else {
            if (scale_factor.x < max_scale.x && scale_factor.y < max_scale.y)
                return scale_factor * 2u;
        }
        return scale_factor;
    }

    // size in number of cells
    size_type size_cells() const
    {
        return size_type(vec::fvec2(size_pixels()) / vec::fvec2(scale_factor));
    }

    size_type size_pixels() const { return size_px; }

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
        position(position() + d);
    }

    // scale, adjusting to maintain position
    void scale_to(size_type coef, position_type focw)
    {
        using vec::fvec2;

        auto coefr = fvec2(coef) / fvec2(max_scale);
        position_type newpos = position_type(coefr * fvec2(position() - focw)) + focw;
        position(newpos);
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
