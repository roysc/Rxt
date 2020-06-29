#version 300 es
precision highp float;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec4 vertex_color;

out vec4 color;
out vec3 position_world;
out vec3 normal_camera;
out vec3 eye_dir_camera;
out vec3 light_dir_camera;

uniform mat4 M;
uniform mat4 V;
uniform mat4 MVP;
uniform vec3 light_position;

void main()
{
    gl_Position = MVP * vec4(vertex_position, 1);

    position_world = (M * vec4(vertex_position, 1)).xyz;
    normal_camera = (V * M * vec4(vertex_normal, 0)).xyz;
    color = vertex_color;

    vec3 pos_camera = (V * M * vec4(vertex_position, 1)).xyz;
    eye_dir_camera = vec3(0) - pos_camera;

    vec3 light_pos_camera = (V * vec4(light_position, 1)).xyz;
    light_dir_camera = light_pos_camera + eye_dir_camera;
}
