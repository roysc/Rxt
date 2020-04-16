#version 300 es
precision highp float;

in vec2 texCoord_;
out vec4 outColor;

uniform sampler2D tex;

void main()
{
    vec4 c = texture(tex, texCoord_);
    outColor = c;
}
