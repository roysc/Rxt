#version 450
precision highp float;

in vec4 color_geom;
out vec4 color_out;

void main()
{
    color_out = color_geom;
}
