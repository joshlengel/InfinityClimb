#version 330 core

layout(location = 0) in vec3 vertex;

out vec3 texture_coords;

uniform mat4 view;
uniform mat4 projection;
uniform float scale;

void main() {
    gl_Position = projection * view * vec4(vertex * scale, 1.0);

    texture_coords = vertex;
}