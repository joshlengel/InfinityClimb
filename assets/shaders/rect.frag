#version 330 core

precision mediump float;

out vec4 color;

uniform vec4 base_color;

void main() {
    color = base_color;
}