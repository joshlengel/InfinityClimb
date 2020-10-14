#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out vec3 pass_vertex;
out vec3 pass_normal;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * transform * vec4(vertex, 1.0);
    pass_vertex = vertex;
    pass_normal = normal;
}