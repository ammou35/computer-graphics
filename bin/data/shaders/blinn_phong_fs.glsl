#version 330 core

#define MAX_LIGHTS 8

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_texcoord;

out vec4 fragColor;

uniform sampler2D tex;

uniform vec3 light_positions[MAX_LIGHTS];  // World-space
uniform vec3 light_colors[MAX_LIGHTS];
uniform int num_lights;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float brightness;

void main()
{
    vec3 norm = normalize(frag_normal);
    vec3 view_dir = normalize(-frag_pos); // camera is at (0,0,0) in world space

    vec3 result = color_ambient;

    for (int i = 0; i < num_lights; ++i) {
        vec3 light_dir = normalize(light_positions[i] - frag_pos);
        vec3 halfway_dir = normalize(light_dir + view_dir);

        float diff = max(dot(norm, light_dir), 0.0);
        float spec = pow(max(dot(norm, halfway_dir), 0.0), brightness);

        vec3 light_color = light_colors[i];
        result += diff * color_diffuse * light_color;
        result += spec * color_specular * light_color;
    }

    vec4 tex_color = texture(tex, frag_texcoord);
    fragColor = vec4(result, 1.0) * tex_color;
}
