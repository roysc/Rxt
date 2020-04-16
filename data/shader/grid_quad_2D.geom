#version 450
precision highp float;

uniform uvec2 viewportSize;

uniform ivec2 CORNERS[4] = ivec2[4](ivec2(0, 1), ivec2(1, 1), ivec2(1, 0), ivec2(0, 0));
uniform uint INDICES[6] = {0, 1, 2, 2, 3, 0};

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

in vec4 color_vert[];
in uvec2 tileSize[];

out vec4 color_geom;

void main()
{
    color_geom = color_vert[0];

    for (int i = 0; i < 6; ++i) {
        vec2 pos = vec2(gl_in[0].gl_Position) + vec2(CORNERS[INDICES[i]] * tileSize[0]);
        pos = 2 * pos / vec2(viewportSize);
        gl_Position = vec4(pos, 0., 1.);
        EmitVertex();
    }
    EndPrimitive();
}
