#version 300 es
precision highp float;

in vec4 color_vert;
out vec4 color_out;

void main()
{
    color_out = color_vert;
}
