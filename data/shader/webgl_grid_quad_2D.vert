#version 300 es
precision highp float;

uniform ivec2 viewportPosition;
uniform uvec2 viewportSize;

in ivec2 quadPosition;
in vec4 color;

out vec4 color_vert;

void main()
{
    vec2 pos = 2. * vec2(quadPosition - viewportPosition) / vec2(viewportSize);
    gl_Position = vec4(pos, 0., 1.);

    color_vert = color;
}
