#version 450
precision highp float;

uniform ivec2 viewportPosition;

in ivec2 quadPosition;
in uvec2 quadSize;
in vec4 color;

out vec4 color_vert;
out uvec2 tileSize;

void main()
{
    vec2 pos = vec2(quadPosition - viewportPosition);
    gl_Position = vec4(pos, 0, 0);

    color_vert = color;
    tileSize = quadSize;
}
