#include "camera.hpp"

namespace Rxt
{
Mat4f focused_camera::projection_matrix() const
{
    // // Projection matrix : 45° Field of View, 4:3 ratio, Z bounds [0.1, 10]
    return glm::perspective(field_of_view, 4.f / 3, 1.f, 100.f);

    // return glm::ortho(0.f, 800.f, 600.f, 0.f, 1.f, 100.f);
    // return glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, 1.f, 10.f);
    // return glm::ortho(-1.f, 1.f, 1.f, -1.f, 1.f, 10.f);

    // FIXME breaks cursor projection
    // const float htow = 3.f/4;   // height to width
    // auto d = distance();
    // return glm::ortho(-d, d, -d * htow, d * htow, -10.f, 10.f);
}
}
