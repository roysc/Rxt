#pragma once

#include "Rxt/util.hpp"
#include "Rxt/graphics/sdl.hpp"
#include "Rxt/graphics/gl.hpp"
#include "Rxt/graphics/camera.hpp"
#include "Rxt/graphics/shader/colored_triangle_3D.hpp"

namespace sdl = Rxt::sdl;
namespace gl = Rxt::gl;
using Rxt::shader_programs::colored_triangle_3D;

struct geom_viewer : public virtual sdl::simple_gui
                   , public virtual Rxt::runtime
{
    sdl::key_dispatcher keys;
    Rxt::focused_camera camera;
    bool dirty = true;
    gl::program_loader loader;
    colored_triangle_3D prog{loader};
    colored_triangle_3D::data b_triangles{prog};

    geom_viewer();

    void advance(SDL_Event);
    void draw();
    void update_camera();
    void update_model();
};
