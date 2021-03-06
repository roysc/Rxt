#include "geom_viewer.hpp"

#include <Rxt/_debug.hpp>
#include <Rxt/graphics/glm.hpp>
#include <Rxt/geometry/helper.hpp>
#include <Rxt/geometry/shapes.hpp>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/IO/Color.h>

#include <iostream>
#include <functional>

extern "C" void step_state(void* c)
{
    sdl::step<geom_viewer>(c);
}

int main()
{
    auto loop = sdl::make_looper(new geom_viewer(), step_state);
    loop();
}

template <class K>
glm::vec3 to_glm(CGAL::Point_3<K> v)  { return {v.x(), v.y(), v.z()}; }
template <class K>
glm::vec3 to_glm(CGAL::Vector_3<K> v) { return {v.x(), v.y(), v.z()}; }

inline glm::vec3 to_glm(CGAL::Color c) { return glm::vec3 {c.red(), c.green(), c.blue()} / 256.f; }
inline glm::vec3 to_glm(glm::vec3 v)   { return v; }

void turn(Rxt::focused_camera& cam, float d, const glm::vec3 axis)
{
    glm::quat rot = glm::angleAxis(d, axis);
    cam.orbit(rot);
}

geom_viewer::geom_viewer()
    : simple_gui{"demo: geom_viewer", 500, 500}
    , camera(glm::vec3{2})
{
    auto camera_right = [this] (float d) {
        turn(camera, 0.1 * d, gl::AXIS_Z);
        update_camera();
    };

    keys.on_scan["Right"] = std::bind(camera_right, +1);
    keys.on_scan["Left"] = std::bind(camera_right, -1);
    keys.on_press["C-W"] = [this] { should_quit(true); };

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    set(b_triangles.light_position, glm::vec3{15, 0, 15});

    update_model();
    update_camera();
}

void geom_viewer::update_camera()
{
    // dbg::print("camera at {}\n", camera.position());

    auto view_matrix  = camera.view_matrix();
    auto model_matrix = camera.model_matrix();
    auto mvp_matrix   = camera.projection_matrix() * view_matrix * model_matrix;

    set(b_triangles.model_matrix, model_matrix);
    set(b_triangles.view_matrix, view_matrix);
    set(b_triangles.mvp_matrix, mvp_matrix);

    set_dirty();
}

void geom_viewer::update_model()
{
    using K = CGAL::Simple_cartesian<double>;
    using Mesh = CGAL::Surface_mesh<K::Point_3>;
    using GT = boost::graph_traits<Mesh>;
    using FaceNormalMap = std::map<typename GT::face_descriptor, K::Vector_3>;

    Mesh sm;
    FaceNormalMap face_normals;

    Rxt::make_icosphere(sm, 1.0, 0);
    Rxt::calculate_face_normals(sm, boost::make_assoc_property_map(face_normals));
    auto pm = get(CGAL::vertex_point, sm);
    unsigned count = 0;

    b_triangles.clear();
    for (auto fd: faces(sm)) {
        auto n = face_normals[fd];
        for (auto vd: vertices_around_face(halfedge(fd, sm), sm)) {
            b_triangles.push(to_glm(pm[vd]), to_glm(n), to_glm(CGAL::green()));
            count++;
        }
        assert(count % 3 == 0);
    }
    b_triangles.update();
    set_dirty();
}

void geom_viewer::draw()
{
    glClearColor(0.69, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    b_triangles.draw();

    SDL_GL_SwapWindow(window.get());
}

void geom_viewer::step(SDL_Event event)
{
    switch (event.type) {
    case SDL_QUIT: { should_quit(true); return; }
    case SDL_KEYDOWN: { keys.press(event.key.keysym); break; }
    }

    keys.scan();

    if (is_dirty()) {
        draw();
        set_dirty(false);
    }
}
