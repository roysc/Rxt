#version 450 core

in vec2 position;
in vec2 texCoord;

out vec2 texCoord_;

void main()
{
    gl_Position = vec4(position, 0, 1);
    texCoord_ = texCoord;
}
