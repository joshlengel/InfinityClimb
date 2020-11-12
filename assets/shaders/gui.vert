#version 330 core

layout(location = 0) in vec2 vertex;

uniform vec4 location;

void main() {
    gl_Position = vec4(vertex * location.zw + location.xy, 0.0, 1.0);
}