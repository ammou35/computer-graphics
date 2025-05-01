#version 330

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

uniform vec3 mat_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float brightness;
uniform vec3 light_ambient;

uniform int num_lights;
uniform vec3 light_positions[30];
uniform vec3 light_colors[30];

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec3 vertex_color;
out vec2 surface_texcoord;

void main() {
    vec3 pos = vec3(modelViewMatrix * vec4(position, 1.0));
    vec3 n = normalize(normalMatrix * normal);
    vec3 v = normalize(-pos);

    vec3 result = light_ambient * mat_ambient;

    for (int i = 0; i < num_lights; ++i) {
        vec3 l = normalize(light_positions[i] - pos);
        vec3 h = normalize(v + l);

        float diff = max(dot(n, l), 0.0);
        float spec = pow(max(dot(n, h), 0.001), brightness);

        vec3 diffuse = diff * color_diffuse * light_colors[i];
        vec3 specular = spec * color_specular * light_colors[i];
        result += diffuse + specular;
    }

    vertex_color = result;
    surface_texcoord = texcoord;
    gl_Position = projectionMatrix * vec4(pos, 1.0);
}
