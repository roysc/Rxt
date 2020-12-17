#pragma once

#include "Rxt/graphics/sdl.hpp"
#include "Rxt/graphics/gl.hpp"
#include "Rxt/graphics/shader/texture_quad_2D.hpp"

#ifdef RXT_USE_GLES3
  #include "Rxt/graphics/shader/webgl_grid_quad_2D.hpp"
#else
  #include "Rxt/graphics/shader/grid_quad_2D.hpp"
#endif

#include "Rxt/util.hpp"
#include "Rxt/range.hpp"

#include <glm/gtx/transform.hpp>

using grid_coord = glm::ivec2;
using grid_size = glm::uvec2;
using grid_offset = glm::ivec2;

namespace gl = Rxt::gl;

using Rxt::shader_programs::texture_quad_2D;
#ifdef RXT_USE_GLES3
using Rxt::shader_programs::webgl::grid_quad_2D;
#else
using Rxt::shader_programs::grid_quad_2D;
#endif

template <class V>
auto invert_y(V v) { v.y = -v.y; return v; }

inline grid_coord nds_to_grid(glm::vec2 nds, glm::vec2 scale) { return floor(nds * scale); }

struct grid_context : public Rxt::sdl::simple_gui
{
    grid_size world_size;
    grid_size viewport_size;
    glm::uvec2 tile_size_px;

    grid_coord viewport_position {0};

    texture_quad_2D tex_prog;
    grid_quad_2D quad_prog;

    texture_quad_2D::buffers b_texs {tex_prog};
    grid_quad_2D::buffers b_quads {quad_prog};

    bool _dirty = true;

    grid_context(const char* title, grid_size world_size, glm::uvec2 tile_px)
        : simple_gui(title, tile_px * world_size)
        , world_size(world_size)
        , viewport_size(world_size)
        , tile_size_px(tile_px)
    {}

    ~grid_context() override {}

    bool is_dirty() const { return _dirty; }
    bool set_dirty(bool v = true)
    {
        bool ret = _dirty;
        _dirty = v;
        return ret;
    }

    static constexpr bool is_torus() { return true; }

    void update_model();
    void update_texture(void*);

    virtual void update_viewport();

    void h_move_viewport(int dx, int dy)
    {
        viewport_position += grid_coord{dx, dy};
        if constexpr (is_torus()) {
            viewport_position %= grid_coord{world_size};
        }
        update_viewport();
    }

    void h_scale_viewport(int);
};

void grid_context::h_scale_viewport(int exp)
{
    // simulate zoom in/out by scaling down/up resp.; correct position to keep centered
    if (exp > 0) {
        if (viewport_size.x >= world_size.x || viewport_size.y >= world_size.y)
            return;
        viewport_size *= 2;
        tile_size_px /= 2;
    } else {
        if (viewport_size.x <= 1 || viewport_size.y <= 1)
            return;
        viewport_size /= 2;
        tile_size_px *= 2;
    }
    update_viewport();
}

void grid_context::update_texture(void* image)
{
    gl::use_guard _g(tex_prog);
    gl::bind_vao_guard _a(b_texs.va);
    glActiveTexture(GL_TEXTURE0);
    gl::bind_texture_guard _t(GL_TEXTURE_2D, b_texs.tex);

    if constexpr (is_torus()) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, world_size.x, world_size.y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image);

    set_dirty();
}

void grid_context::update_model()
{
    b_texs.position.storage = {{-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
    b_texs.tex_coord.storage = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
    b_texs.elements.storage = {0, 1, 2, 2, 3, 0};
    b_texs.update();

    // gl::set_uniform(quad_prog, "worldSize", world_size); // will need for instancing

    set_dirty();
}

void grid_context::update_viewport()
{
    using glm::vec2;
    using glm::vec3;

    vec2 vp_rel_size = vec2(viewport_size) / vec2(world_size);
    vec2 vp_rel_pos = vec2(viewport_position) / vec2(world_size);
    glm::mat4 tex_view_matrix =
        glm::translate(vec3(vp_rel_pos, 0)) *
        glm::scale(vec3(invert_y(vp_rel_size), 0)) *
        glm::translate(vec3(-.5, -.5, 0));

    set_uniform(tex_prog, "viewMatrix", tex_view_matrix);

    set_uniform(quad_prog, "viewportPosition", viewport_position);
    set_uniform(quad_prog, "viewportSize", viewport_size);

    set_dirty();
}
