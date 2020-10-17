#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;

out vec3 pass_vertex;
out vec3 pass_normal;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 transformed_pos = transform * vec4(vertex, 1.0); 
    gl_Position = projection * view * transformed_pos;
    pass_vertex = transformed_pos.xyz;
    pass_normal = normalize((transform * vec4(normal, 0.0)).xyz);
}