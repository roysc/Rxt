#pragma once
#include "../_debug.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cmath>

namespace Rxt
{
namespace _gfx
{
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::quat;

inline
quat rotation_between(vec3 v0, vec3 v1)
{
    auto l0 = length(v0) * length(v0);
    auto l1 = length(v1) * length(v1);
    vec3 a = cross(v0, v1);
    quat q {std::sqrt(l0 * l1) + dot(v0, v1), a.x, a.y, a.z};

    return normalize(q);
}

struct focused_camera
{
    static constexpr float field_of_view = M_PI/4;

    vec3 _position;
    vec3 focus;
    vec3 up;

    focused_camera(vec3 pos = vec3{0}, vec3 f = vec3{0}, vec3 u = vec3{0, 0, 1})
        : _position(pos)
        , focus(f)
        , up(u)
    { }

    vec3 position() const { return _position; }
    void position(vec3 pos) { _position = pos; }

    // Rotate about focal point
    void orbit(quat rot)
    {
        auto rotmat = glm::mat4_cast(rot);
        _position = vec3{rotmat * vec4{_position - focus, 0}} + focus;
        up = vec3{rotmat * vec4{up, 0}};
    }

    void translate(vec3 t)
    {
        auto tmat = glm::translate(t);
        _position = vec3{tmat * vec4{_position, 1}};
        focus = vec3{tmat * vec4{focus, 1}};
    }

    void forward(float d)
    {
        auto tmat = glm::translate(d * orientation());
        _position = vec3{tmat * vec4{_position, 1}};
    }

    mat4 model_matrix() const
    {
        return glm::translate(- _position);
    }

    mat4 view_matrix() const
    {
        return glm::lookAt(_position, focus, up);
    }

    mat4 projection_matrix() const
    {
        // FOV, aspect ratio, Z near, Z far
        return glm::perspective(field_of_view, 1.f, 1.f, 100.f);
    }

    vec3 orientation() const
    {
        return normalize(focus - _position);
    }
};

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

// Source: http://antongerdelan.net/opengl/raycasting.html
// Given a coordinate in NDS and a camera object, returns ray pointing into space
// as (source point, normalized direction vector)
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
}

using _gfx::focused_camera;
using _gfx::unview;
using _gfx::unproject;
using _gfx::cast_ray;
using _gfx::rotation_between;
}
