#version 330 core

layout(location=0) in vec2 vertex;
layout(location=1) in vec2 tex_coords;

out vec2 pass_tex_coords;

uniform float scale;
uniform float aspect_ratio;

void main() {
    vec2 position = vertex * scale;
    position.x /= aspect_ratio;

    gl_Position = vec4(position, 0.0, 1.0);

    pass_tex_coords = tex_coords;
}