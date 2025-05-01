#version 330 core

flat in vec3 flat_normal;
in vec3 frag_pos;

out vec4 fragColor;

uniform vec3 light_position;  // en espace vue
uniform vec3 color_diffuse;
uniform vec3 color_ambient;

void main() {
    vec3 light_dir = normalize(light_position - frag_pos);
    float diff = max(dot(flat_normal, light_dir), 0.0);

    vec3 result = color_ambient + diff * color_diffuse;
    fragColor = vec4(result, 1.0);
}
