#version 320 es
precision highp float;

in vec3 color;

out vec4 outColor;

void main()
{
    outColor = vec4(color, 1.0);
}
