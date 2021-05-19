#version 320 es
precision highp float;

in vec4 color;
in vec3 position_world;
in vec3 normal_camera;
in vec3 eye_dir_camera;
in vec3 light_dir_camera;

out vec4 out_color;

uniform vec3 light_position;

void main()
{
    vec3 light_color = vec3(1, 1, 1);
    float light_power = 500.0f;

    vec3 material_diffuse_color = color.rgb;
    vec3 material_ambient_color = vec3(0.1) * material_diffuse_color;
    vec3 material_specular_color = vec3(0.3);

    vec3 N = normalize(normal_camera);
    vec3 L = normalize(light_dir_camera);
    vec3 E = normalize(eye_dir_camera);
    vec3 R = reflect(-L, N);
    float cos_theta = clamp(dot(N, L), 0., 1.);
    float cos_alpha = clamp(dot(E, R), 0., 1.);

    vec3 to_light = light_position - position_world;
    float light_dist2 = dot(to_light, to_light);

    out_color.rgb =
        material_ambient_color +
        material_diffuse_color * light_color * light_power * cos_theta / light_dist2 +
        material_specular_color * light_color * light_power * pow(cos_alpha, 5.) / light_dist2;
    out_color.a =
        color.a;
}
