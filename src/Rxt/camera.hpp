#pragma once

#include "Rxt/math.hpp"
#include "Rxt/vec.hpp"

#include <utility>

namespace Rxt
{
struct focused_camera
{
    using position_type = vec::fvec3;
    using vector_type = position_type;
    using matrix_type = vec::fmat4;

    using M = matrix_type;
    using P = position_type;
    using V = vector_type;
    using H = vec::fvec4;

    inline static const vector_type default_up {0, 0, 1};
    static constexpr float field_of_view = tau/8;
    // static constexpr float epsilon = 0.001f;

    P _position;
    P focus;
    V up;

    focused_camera(P pos, P f = P{0}, V u = default_up)
        : _position(pos)
        , focus(f)
        , up(u)
    {}

    virtual void set_position(P pos) { _position = pos; }

    P position() const { return _position; }

    // Rotate about focal point
    void orbit(vec::quat rot)
    {
        auto rotmat = mat4_cast(rot);
        set_position(P(rotmat * H(position() - focus, 0)) + focus);
        up = P(rotmat * H(up, 0));
    }

    void translate(V t)
    {
        auto tmat = vec::translate(t);
        set_position(P(tmat * H(position(), 1)));
        focus = P(tmat * H(focus, 1));
    }

    void forward(float d)
    {
        auto newpos = position() + d * orientation();
        set_position(newpos);
    }

    M model_matrix() const
    {
        return vec::translate(- _position);
    }

    M view_matrix() const
    {
        return lookAt(_position, focus, up);
    }

    M projection_matrix() const
    {
        // FOV, aspect ratio, Z near, Z far
        return glm::perspective(field_of_view, 1.f, 1.f, 100.f);
    }

    V orientation() const
    {
        return normalize(offset());
    }

    V offset() const
    {
        return focus - position();
    }

    virtual ~focused_camera() {}
};

namespace _det
{
using namespace vec;

template <class Cam>
fvec3 unview(fvec4 view, Cam const& cam)
{
    return fvec3{inverse(cam.view_matrix()) * view};
}

template <class Cam>
fvec4 unproject(fvec4 hcs, Cam const& cam)
{
    return fvec4{inverse(cam.projection_matrix()) * hcs};
}

// Given a coordinate in NDS and a camera object, returns ray pointing into space
// as (source point, normalized direction vector)
// Source: http://antongerdelan.net/opengl/raycasting.html
template <class Cam>
std::pair<fvec3, fvec3> cast_ray(fvec2 coord_nds, Cam const& cam)
{
    // Homogeneous clip space (projected) -> View -> Model [-> World]
    fvec4 ray_hcs {coord_nds, -1, 0};
    fvec4 ray_view = unproject(ray_hcs, cam);
    ray_view = fvec4{ray_view.x, ray_view.y, -1, 0};

    fvec3 ray_model = unview(ray_view, cam);
    ray_view = fvec4{ray_view.x, ray_view.y, -1, 1};
    fvec3 eye_model = unview(ray_view, cam);
    fvec3 eye_world = cam.position() + eye_model;

    return {eye_world, normalize(ray_model)};
}

inline
quat rotation_between(fvec3 v0, fvec3 v1)
{
    auto l0 = length(v0) * length(v0);
    auto l1 = length(v1) * length(v1);
    fvec3 a = cross(v0, v1);
    quat q {std::sqrt(l0 * l1) + dot(v0, v1), a.x, a.y, a.z};

    return normalize(q);
}
}

using _det::unview;
using _det::unproject;
using _det::cast_ray;
using _det::rotation_between;
}
