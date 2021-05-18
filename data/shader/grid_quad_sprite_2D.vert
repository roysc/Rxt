#version 300 es
precision highp float;

#ifndef MAX_SPRITES
#define MAX_SPRITES 128
#endif

uniform ivec2 viewportPosition;
uniform uvec2 viewportSize;
uniform ivec2 offsets[MAX_SPRITES];

in ivec2 vertexPosition;
in ivec2 texCoord;
in uint spriteIndex;

out vec2 texCoord_vert;

void main()
{
    vec2 pos = 2. * vec2(vertexPosition + offsets[spriteIndex] - viewportPosition) / vec2(viewportSize);
    gl_Position = vec4(pos, 0., 1.);

    texCoord_vert = vec2(texCoord);
}
