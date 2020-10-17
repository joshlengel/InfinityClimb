#version 330 core

in vec3 pass_vertex;
in vec3 pass_normal;

precision mediump float;

out vec4 color;

uniform vec4 base_color;
uniform vec3 light_position;
uniform vec3 camera_position;

const float shine_damp = 5.0;
const float reflectivity = 0.1;

float get_specular_light_factor(vec3 to_camera, vec3 reflected)
{
    return reflectivity * pow(max(dot(to_camera, reflected), 0.0), shine_damp);
}

void main() {
    vec3 to_light = normalize(light_position - pass_vertex);
    vec3 to_camera = normalize(camera_position - pass_vertex);

    vec3 reflected = reflect(-to_light, pass_normal);

    float diffuse_factor = max(dot(pass_normal, to_light), 0.2);
    float specular_factor = get_specular_light_factor(to_camera, reflected);

    color = base_color * diffuse_factor + vec4(1.0) * specular_factor;
}