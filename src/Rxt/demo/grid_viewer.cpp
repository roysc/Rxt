#include "Rxt/graphics/loader.hpp"
#include "Rxt/graphics/images.hpp"
#include "Rxt/graphics/sdl.hpp"
#include "Rxt/graphics/gl.hpp"
#include "Rxt/graphics/gl_handy.hpp"

namespace sdl = Rxt::sdl;
namespace gl = Rxt::gl;
// using Rxt::print;

struct run_context : public Rxt::file_loader
{
    sdl::window_ptr window;
    sdl::key_dispatcher kd;
    bool _running = true;
    bool _dirty = true;

    gl::program prog;
    gl::vao va;
    gl::texture tex;
    gl::attribuf<glm::vec2> position;
    gl::attribuf<glm::vec2> tex_coord;
    gl::buffer<GLuint> elements;

    run_context(sdl::window_ptr, unsigned, unsigned);
    ~run_context();
    bool should_quit() const { return !_running; }
    void step();

    void update_model();
    void update_texture();
    void draw();
};

extern "C" void step_state(void* c)
{
    sdl::step<run_context>(c);
}

int main()
{
    glm::uvec2 num_tiles {16, 16};
    glm::uvec2 tile_size_px {16, 16};
    glm::uvec2 total_size_px = tile_size_px * num_tiles;

    sdl::simple_gui gui("demo: grid viewer", total_size_px.x, total_size_px.y);
    auto loop = sdl::make_looper(new run_context(gui.window, num_tiles.x, num_tiles.y), step_state);
    loop();
}

run_context::run_context(sdl::window_ptr w, unsigned width, unsigned height)
    : window(w)
    , prog(find_program("grid_viewer"))
    , position(prog, "position")
    , tex_coord(prog, "texCoord")
{
    kd.on_press["R"] = [this] { _dirty = true; };

    glUseProgram(prog);

    glBindVertexArray(va);
    gl::attrib_buffer(position);
    glEnableVertexArrayAttrib(va, position);
    gl::attrib_buffer(tex_coord);
    glEnableVertexArrayAttrib(va, tex_coord);

    gl::set_uniform(prog, "tex", 0);
    update_texture();
}

run_context::~run_context()
{
    glUseProgram(0);
}

void run_context::update_texture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    using Vec4u8 = glm::tvec4<GLubyte, glm::packed_highp>;
    int width = 8, height = 8;
    std::vector<Vec4u8> image;
    for (int w = 0; w < width; ++w) {
        for (int h = 0; h < height; ++h) {
            image.emplace_back(static_cast<GLubyte>((w + h) % 2 == 0) * 0xFF);
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data(image));

    glBindTexture(GL_TEXTURE_2D, 0);
}

void run_context::update_model()
{
    position.storage = {{-1, 1}, {1, 1}, {1, -1}, {-1, -1}};
    tex_coord.storage = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
    elements.storage = {0, 1, 2, 2, 3, 0};

    gl::buffer_data(elements, GL_ELEMENT_ARRAY_BUFFER);
    gl::buffer_data(position);
    gl::buffer_data(tex_coord);
}

void run_context::draw()
{
    glClearColor(0, 0.69, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, tex);
    glBindVertexArray(va);
    glDrawElements(GL_TRIANGLES, std::size(elements.storage), GL_UNSIGNED_INT, 0);

    SDL_GL_SwapWindow(window.get());
}

void run_context::step()
{
    SDL_Event event;
    SDL_WaitEvent(&event);

    kd.scan();

    switch (event.type) {
    case SDL_QUIT: { _running = false; return; }
    case SDL_KEYDOWN: { kd.press(event.key.keysym); break; }
    }

    if (_dirty) {
        update_model();
        draw();
        _dirty = false;
    }
}
