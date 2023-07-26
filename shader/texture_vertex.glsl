#version 410 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in uint a_texture_index;
layout (location = 3) in vec2 a_texture_coordinate;

flat out uint texture_index;
out vec2 texture_coordinate;
out vec4 lighting_color;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 light_pos;
uniform vec3 view_pos;

void main() {
    vec3 light_color = vec3(1.0, 1.0, 1.0);

    vec3 ambient = 0.1 * light_color;

    vec3 light_direction = normalize(light_pos - a_pos);
    vec3 diffuse = max(dot(a_normal, light_direction), 0.0) * light_color;

    vec3 view_direction = normalize(view_pos - a_pos);
    vec3 reflect_direction = reflect(-light_direction, a_normal);
    vec3 specular = 0.5 * pow(max(dot(view_direction, reflect_direction), 0.0), 32) * light_color;

    float frag_distance = length(light_pos - a_pos);
    float light_constant = 1.0;
    float light_linear = 0.022;
    float light_quadratic = 0.0019;
    float attenuation = 1.0 / (light_constant + (light_linear * frag_distance) + (light_quadratic * frag_distance * frag_distance));

    texture_index = a_texture_index;
    texture_coordinate = a_texture_coordinate;
    lighting_color = vec4((ambient + diffuse + specular) * attenuation, 1.0);
    gl_Position = projection * view * vec4(a_pos, 1.0);
}
