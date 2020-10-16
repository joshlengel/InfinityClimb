#version 330 core

in vec3 pass_vertex;
in vec3 pass_normal;

precision mediump float;

out vec4 color;

uniform vec4 base_color;
uniform vec3 light_dir;

void main() {
    float light_factor = max(dot(pass_normal, -light_dir), 0.2);

    color = base_color * light_factor;
}