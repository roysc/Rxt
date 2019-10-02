#pragma once
#include "../_debug.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cmath>

namespace Rxt
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

    vec3 position;
    vec3 focus;
    vec3 up;

    focused_camera(vec3 pos, vec3 f = vec3 {0}, vec3 u = vec3 {0, 0, 1})
        : position(pos)
        , focus(f)
        , up(u)
    { }

    // Rotate about focal point
    void rotate(quat rot)
    {
        auto rotmat = glm::mat4_cast(rot);
        position = vec3 {rotmat * vec4 {position - focus, 0}} + focus;
        up = vec3 {rotmat * vec4{up, 0}};
    }

    void translate(vec3 t)
    {
        auto tmat = glm::translate(t);
        position = vec3 {tmat * vec4 {position, 1}};
        focus = vec3 {tmat * vec4 {focus, 1}};
    }

    void forward(float d)
    {
        auto tmat = glm::translate(d * orientation());
        position = vec3 {tmat * vec4 {position, 1}};
    }

    mat4 model_matrix() const
    {
        return glm::translate(- position);
    }

    mat4 view_matrix() const
    {
        return glm::lookAt(position, focus, up);
    }

    mat4 projection_matrix() const
    {
        // FOV, aspect ratio, Z near, Z far
        return glm::perspective(field_of_view, 1.f, 1.f, 100.f);
    }

    vec3 orientation() const
    {
        return normalize(focus - position);
    }
};


vec3 unview(vec4 view, mat4 view_matrix)
{
    return vec3 {inverse(view_matrix) * view};
}

vec4 unproject(vec4 hcs, mat4 projection_matrix)
{
    return vec4 {inverse(projection_matrix) * hcs};
}

// Source: http://antongerdelan.net/opengl/raycasting.html
std::pair<vec3, vec3> raycast(vec2 point_nds, vec3 cam, mat4 vm, mat4 pm)
{
    // Homogeneous clip space (projected) -> View space -> Camera/world
    vec4 ray_hcs {point_nds, -1, 0};
    vec4 ray_view = unproject(ray_hcs, pm);
    ray_view = vec4{ray_view.x, ray_view.y, -1, 0};

    vec3 ray_cam = unview(ray_view, vm);
    ray_view = vec4{ray_view.x, ray_view.y, -1, 1};
    vec3 eye_cam = unview(ray_view, vm);
    vec3 eye_world = cam + eye_cam;
    // return points describing a ray A->B
    return {eye_world, eye_world + normalize(ray_cam)};
}

}
