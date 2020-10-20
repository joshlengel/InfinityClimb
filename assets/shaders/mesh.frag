#version 330 core

in vec3 pass_vertex;
in vec3 pass_normal;
in vec3 camera_position;

precision mediump float;

out vec4 color;

uniform vec4 base_color;

const vec3 light_color = vec3(1.0, 1.0, 1.0);
const vec3 light_dir = normalize(vec3(-0.4, -0.8, 0.6));

void main() {
    float diffuse_factor = max(dot(-light_dir, pass_normal), 0.2);
    vec4 diffuse_color = vec4(light_color * diffuse_factor, 1.0);

    color = diffuse_color * base_color;
}