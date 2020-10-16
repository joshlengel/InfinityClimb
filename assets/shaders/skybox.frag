#version 330 core

precision mediump float;

in vec3 texture_coords;

out vec4 color;

uniform samplerCube skybox;

void main() {
    color = texture(skybox, texture_coords);
}