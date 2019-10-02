#pragma once
#include "Rxt/graphics/sdl.hpp"
#include "Rxt/graphics/gl.hpp"
#include "Rxt/graphics/gl_handy.hpp"
#include "Rxt/graphics/camera.hpp"
#include "Rxt/graphics/loader.hpp"

namespace sdl = Rxt::sdl;
namespace gl = Rxt::gl;

struct run_context : public Rxt::file_loader
{
    sdl::window_ptr window;
    sdl::key_dispatcher kd;
    Rxt::focused_camera camera;
    bool _quit = false;
    bool _dirty = true;

    using AB = gl::attribuf<glm::vec3>;
    gl::program prog;
    gl::vao va;
    gl::vbo vb;
    AB position, normal, color;

    run_context(sdl::window_ptr);
    ~run_context();
    bool should_quit() const { return _quit; }
    void step();

    void draw();
    void update_camera();
    void update_model();
};
