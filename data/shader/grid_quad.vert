#version 450

uniform ivec2 viewportPosition;
uniform vec2 viewportScale;

in ivec2 gridPosition;
in ivec2 quadSize;
in vec4 color;

out vec4 color_vert;
out vec2 tileSize;

void main()
{
    vec2 p = (gridPosition - viewportPosition) / vec2(viewportScale);
    gl_Position = vec4(p, 0, 1);

    color_vert = color;
    tileSize = vec2(quadSize) / viewportScale;
}
