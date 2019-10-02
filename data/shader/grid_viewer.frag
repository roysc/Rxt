#version 450 core

in vec2 texCoord_;
out vec4 outColor;

uniform sampler2D tex;

void main()
{
    // outColor.rgb = vec3(1);
    vec4 c = texture(tex, texCoord_);
    // outColor.rgb = vec3(1, 0, c.b);
    // outColor = vec3(1, 0, c.b, 1);
    outColor = c;
}
