#version 300 es
precision highp float;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

out vec3 color;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(vertexPosition, 1);
    gl_PointSize = 2.0;
    color = vertexColor;
}
