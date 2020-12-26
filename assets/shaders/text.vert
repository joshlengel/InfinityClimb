#version 330 core

layout(location = 0) in vec2 vertex;
layout(location = 1) in vec2 tex_coords;

out vec2 pass_tex_coords;

uniform float aspect_ratio;

void main() {
    gl_Position = vec4(vertex, 0.0, 1.0);
    gl_Position.x /= aspect_ratio;
    pass_tex_coords = tex_coords;
}