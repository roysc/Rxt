#version 320 es
precision highp float;

uniform ivec2 viewportPosition;
uniform uvec2 viewportSize;

in ivec2 vertexPosition;
in ivec2 texCoord;

out vec2 texCoord_vert;

void main()
{
    vec2 pos = 2. * vec2(vertexPosition - viewportPosition) / vec2(viewportSize);
    gl_Position = vec4(pos, 0., 1.);

    texCoord_vert = vec2(texCoord);
}
