#version 300 es
precision highp float;

in vec2 texCoord_vert;
out vec4 color_out;

uniform sampler2D texUnit;

void main()
{
    color_out = texture(texUnit, texCoord_vert);
}
