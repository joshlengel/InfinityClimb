#version 330 core

precision mediump float;

in vec2 pass_tex_coords;

out vec4 color;

uniform sampler2D tex;

uniform vec4 base_color;

void main() {
    color = base_color * texture(tex, pass_tex_coords).a;
}