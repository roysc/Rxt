#pragma once
#include "Rxt/vec.hpp"

#include <type_traits>

namespace Rxt
{
template <class Vec2>
struct basic_viewport
{
    // using traits_type = ST;
    using position_type = Vec2;
    using size_type = Vec2;
    using fvec2 = vec::fvec2;

    Vec2 max_scale;
    Vec2 scale_factor {1};
    Vec2 _position {0};
    const Vec2 size_px {max_scale * scale_factor};
    const float margin_size = .1;

    basic_viewport(Vec2 max, Vec2 scale = Vec2(1))
        : max_scale{max}, scale_factor{scale}
    {
        assert(all(lessThan(Vec2(0), scale)));
        assert(all(lessThan(Vec2(0), max)));
    }

    virtual void position(Vec2 pos) { _position = pos; }
    virtual void set_scale(Vec2 scale) { scale_factor = scale; }

    Vec2 position() const { return _position; }

    Vec2 scale_relative(float factor) const
    {
        return scale_factor * factor;
    }

    Vec2 scale_pow(int exp) const
    {
        const Vec2 min_scale{1};
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
    Vec2 size_cells() const
    {
        return Vec2(fvec2(size_pixels()) / fvec2(scale_factor));
    }

    Vec2 size_pixels() const { return size_px; }

    auto from_nds(float x, float y) const
    {
        return floor(fvec2(x, y) * fvec2(size_cells() / 2u));
    }

    fvec2 to_nds(Vec2 p) const { return fvec2(p) / fvec2(size_cells() / 2u); }

    vec::fmat4 view_matrix() const
    {
        using vec::fvec3;

        auto pos3 = fvec3(position(), 0);
        vec::fmat4 view_matrix =
            glm::scale(fvec3(2.f / fvec2(size_cells()), 0)) *
            glm::translate(-pos3);
        return view_matrix;
    }

    vec::fmat4 projection_matrix() const
    {
        auto pos = position();
        auto corner = position() + size_cells();
        return glm::ortho(pos.x, corner.x, pos.y, corner.y);
    }

    void translate(Vec2 d)
    {
        position(position() + d);
    }

    // scale, adjusting to maintain position
    void scale_to(Vec2 coef, Vec2 focw)
    {
        auto coefr = fvec2(coef) / fvec2(max_scale);
        Vec2 newpos = Vec2(coefr * fvec2(position() - focw)) + focw;
        position(newpos);
        set_scale(coef);
    }

    bool edge_scroll(Vec2 cursor_position, int speed)
    {
        // (0,0) is center-screen, so offset it to the corner
        auto vpsize = size_cells();
        auto offset_pos = to_nds(cursor_position + Vec2(vpsize / 2u));
        Vec2 dv {0};

        for (unsigned i = 0; i < dv.length(); ++i) {
            if (offset_pos[i] < margin_size) {
                dv[i] = -speed;
                // dv[dv.offset_pos < margin_size] = -speed;
            } else if (offset_pos[i] + margin_size >= 2) {
                dv[i] = +speed;
            }
        }
        if (dv != Vec2(0)) {
            translate(dv);
            return true;
        }
        return false;
    }

    template <class Prog>
    void update_uniforms(Prog& prog, bool pos = true)
    {
        set(prog->viewport_size, size_cells());
        if (pos)
            set(prog->viewport_position, position());
    }
};
}
