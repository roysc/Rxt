#version 320 es
precision highp float;

uniform sampler2D texUnit;

in vec2 texCoord_vert;
out vec4 color_out;

void main()
{
    color_out = texelFetch(texUnit, ivec2(texCoord_vert), 0);
}
