#version 300 es
precision highp float;

in vec2 position;
in vec2 texCoord;

out vec2 texCoord_;

void main()
{
    gl_Position = vec4(position, 0.0, 1.0);
    texCoord_ = texCoord;
}
