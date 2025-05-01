#version 330

in vec3 surface_position;
in vec3 surface_normal;
in vec2 surface_texcoord;

out vec4 fragment_color;

uniform vec3 mat_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float brightness;
uniform vec3 light_ambient;

uniform int num_lights;
uniform vec3 light_positions[30];
uniform vec3 light_colors[30];

uniform sampler2D tex0;

void main() {
    vec3 n = normalize(surface_normal);
    vec3 v = normalize(-surface_position);
    vec3 tex_color = texture(tex0, surface_texcoord).rgb;

    vec3 result = light_ambient * mat_ambient;

    for (int i = 0; i < num_lights; ++i) {
        vec3 l = normalize(light_positions[i] - surface_position);
        vec3 r = reflect(-l, n);

        float diff = max(dot(n, l), 0.0);
        float spec = pow(max(dot(r, v), 0.001), brightness);

        vec3 diffuse = diff * color_diffuse * light_colors[i];
        vec3 specular = spec * color_specular * light_colors[i];

        result += diffuse + specular;
    }

    fragment_color = vec4(tex_color * result, 1.0);
}
