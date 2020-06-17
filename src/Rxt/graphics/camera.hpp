#pragma once
#include "../_debug.hpp"
#include "glm.hpp"

#include <cmath>

namespace Rxt
{

struct focus_cam
{
    static constexpr float field_of_view = M_PI/4;
    using position_type = glm::vec3;
    using matrix_type = glm::mat4;
    using M = matrix_type;
    using P = position_type;
    using H = glm::vec4;

    P _position;
    P focus;
    P up;

    focus_cam(P pos = P{0}, P f = P{0}, P u = P{0, 0, 1})
        : _position(pos)
        , focus(f)
        , up(u)
    { }

    P position() const { return _position; }
    virtual void position(P pos) {_position = pos;}

    // Rotate about focal point
    void orbit(glm::quat rot)
    {
        auto rotmat = glm::mat4_cast(rot);
        position(P{rotmat * H{position() - focus, 0}} + focus);
        up = P{rotmat * H{up, 0}};
    }

    void translate(P t)
    {
        auto tmat = glm::translate(t);
        position(P{tmat * H{position(), 1}});
        focus = P{tmat * H{focus, 1}};
    }

    void forward(float d)
    {
        auto tmat = glm::translate(d * orientation());
        position(P{tmat * H{position(), 1}});
    }

    M model_matrix() const
    {
        return glm::translate(- _position);
    }

    M view_matrix() const
    {
        return glm::lookAt(_position, focus, up);
    }

    M projection_matrix() const
    {
        // FOV, aspect ratio, Z near, Z far
        return glm::perspective(field_of_view, 1.f, 1.f, 100.f);
    }

    P orientation() const
    {
        return normalize(focus - _position);
    }

    virtual ~focus_cam() {}
};
using focused_camera = focus_cam;

namespace _det
{
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::quat;

// template <class Der>
// struct focus_cam_crt : focus_cam
// {
//     using focus_cam::focus_cam;
//     using focus_cam::position;
//     // crt api
//     void position(vec3 pos) { static_cast<Der&>(*this).set_position(pos); }
// };

template <class Cam>
vec3 unview(vec4 view, Cam const& cam)
{
    return vec3{inverse(cam.view_matrix()) * view};
}

template <class Cam>
vec4 unproject(vec4 hcs, Cam const& cam)
{
    return vec4{inverse(cam.projection_matrix()) * hcs};
}

// Given a coordinate in NDS and a camera object, returns ray pointing into space
// as (source point, normalized direction vector)
// Source: http://antongerdelan.net/opengl/raycasting.html
template <class Cam>
std::pair<vec3, vec3> cast_ray(vec2 coord_nds, Cam const& cam)
{
    // Homogeneous clip space (projected) -> View space -> Camera/world
    vec4 ray_hcs {coord_nds, -1, 0};
    vec4 ray_view = unproject(ray_hcs, cam);
    ray_view = vec4{ray_view.x, ray_view.y, -1, 0};

    vec3 ray_cam = unview(ray_view, cam);
    ray_view = vec4{ray_view.x, ray_view.y, -1, 1};
    vec3 eye_cam = unview(ray_view, cam);
    vec3 eye_world = cam.position() + eye_cam;

    return {eye_world, normalize(ray_cam)};
}

inline
quat rotation_between(vec3 v0, vec3 v1)
{
    auto l0 = length(v0) * length(v0);
    auto l1 = length(v1) * length(v1);
    vec3 a = cross(v0, v1);
    quat q {std::sqrt(l0 * l1) + dot(v0, v1), a.x, a.y, a.z};

    return normalize(q);
}
}

using _det::unview;
using _det::unproject;
using _det::cast_ray;
using _det::rotation_between;
}
