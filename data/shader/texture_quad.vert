#version 450

uniform mat4 viewMatrix;

in vec2 position;
in vec2 texCoord;

out vec2 texCoord_vert;

void main()
{
    gl_Position = vec4(position, 0, 1);
    texCoord_vert = vec2(viewMatrix * vec4(texCoord, 0, 1));
}
