#include "geom_viewer.hpp"

#include <Rxt/_debug.hpp>
#include <Rxt/graphics/glm.hpp>
#include <Rxt/geometry/helper.hpp>
#include <Rxt/geometry/shapes.hpp>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <iostream>
#include <functional>

extern "C" void step_state(void* c)
{
    sdl::step<run_context>(c);
}

int main()
{
    sdl::simple_gui gui("demo: geom viewer", 500, 500);
    auto loop = sdl::make_looper(new run_context {gui.window}, step_state);
    loop();
}

// { // TODO move to ... where?
template <class K>
glm::vec3 to_glm(CGAL::Point_3<K> v)  { return {v.x(), v.y(), v.z()}; }
template <class K>
glm::vec3 to_glm(CGAL::Vector_3<K> v) { return {v.x(), v.y(), v.z()}; }

inline glm::vec3 to_glm(CGAL::Color c) { return glm::vec3 {c.red(), c.green(), c.blue()} / 256.f; }
inline glm::vec3 to_glm(glm::vec3 v)   { return v; }
// }

void turn(Rxt::focused_camera& cam, float d, const glm::vec3 axis)
{
    glm::quat rot = glm::angleAxis(d, axis);
    cam.rotate(rot);
}

run_context::run_context(sdl::window_ptr w)
    : window(w)
    , camera(glm::vec3 {2})
    , prog(find_program("basic3d"))
    , position(prog, "vertex_position")
    , normal(prog, "vertex_normal")
    , color(prog, "vertex_color")
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glEnable(GL_PROGRAM_POINT_SIZE);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto camera_right =
        [this] (float d) {
            turn(camera, 0.1 * d, gl::AXIS_Z);
            update_camera();
            draw();
        };

    kd.on_scan["Right"] = std::bind(camera_right, +1);
    kd.on_scan["Left"] = std::bind(camera_right, -1);
    kd.on_press["C-W"] = [this] { _quit = true; };

    glUseProgram(prog);

    gl::uniform<glm::vec3> light(prog, "light_position");
    set(light, glm::vec3 {15, 0, 15});

    update_camera();

    glBindVertexArray(va);
    for (auto* ab: {&position, &normal, &color}) {
        attrib_buffer(*ab);
        glEnableVertexArrayAttrib(va, *ab);
    }
}

run_context::~run_context()
{
    glUseProgram(0);
}

void run_context::update_camera()
{
    // std::cout << "camera=" << camera.position << "\n";

    gl::uniform<glm::mat4> m(prog, "M"), v(prog, "V"), mvp(prog, "MVP");

    auto view_matrix  = camera.view_matrix();
    auto model_matrix = camera.model_matrix();
    auto mvp_matrix   = camera.projection_matrix() * view_matrix * model_matrix;

    set(m, model_matrix);
    set(v, view_matrix);
    set(mvp, mvp_matrix);
}

void run_context::update_model()
{
    // {
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
    for (auto fd: faces(sm)) {
        auto n = face_normals[fd];
        for (auto vd: vertices_around_face(halfedge(fd, sm), sm)) {
            position.storage.emplace_back(to_glm(pm[vd]));
            normal.storage.emplace_back(to_glm(n));
            color.storage.emplace_back(to_glm(CGAL::GREEN));
            count++;
        }
        assert(count % 3 == 0);
    }
    // }

    for (auto* ab: {&position, &normal, &color}) {
        gl::buffer_data(*ab);
    }
}

void run_context::draw()
{
    glClearColor(0.69, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, std::size(position.storage));

    SDL_GL_SwapWindow(window.get());
}

void run_context::step()
{
    SDL_Event event;
    SDL_WaitEvent(&event);

    kd.scan();

    switch (event.type) {
    case SDL_QUIT: { _quit = true; return; }
    case SDL_KEYDOWN: { kd.press(event.key.keysym); break; }
    }

    if (_dirty) {
        _dirty = false;
        update_model();
        draw();
    }
}
