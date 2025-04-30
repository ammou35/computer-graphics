#version 330 core

#define MAX_LIGHTS 8

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_texcoord;

out vec4 fragColor;

uniform sampler2D tex;

struct Light {
    int type;           // 0: ambient, 1: directional, 2: point, 3: spot
    vec3 position;
    vec3 direction;
    vec3 color;
    float cutoff;       // for spot (cosine of angle)
};

uniform Light lights[MAX_LIGHTS];
uniform int num_lights;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float brightness;

void main()
{
    vec3 norm = normalize(frag_normal);
    vec3 view_dir = normalize(-frag_pos);
    vec3 result = color_ambient;

    for (int i = 0; i < num_lights; ++i) {
        if (lights[i].type == 0) {
            result += lights[i].color * color_ambient;
            continue;
        }

        vec3 light_dir;
        if (lights[i].type == 1) {
            light_dir = normalize(-lights[i].direction); // Directionnelle
        } else {
            light_dir = normalize(lights[i].position - frag_pos); // Point ou spot
        }

        float diff = max(dot(norm, light_dir), 0.0);
        float spec = 0.0;
        if (diff > 0.0) {
            vec3 halfway = normalize(light_dir + view_dir);
            spec = pow(max(dot(norm, halfway), 0.0), brightness);
        }

        float attenuation = 1.0;
        if (lights[i].type == 3) {
            float theta = dot(normalize(-lights[i].direction), normalize(frag_pos - lights[i].position));
            if (theta < lights[i].cutoff) {
                attenuation = 0.0;
            }
        }

        vec3 light_contrib = attenuation * lights[i].color * (diff * color_diffuse + spec * color_specular);
        result += light_contrib;
    }

    vec4 tex_color = texture(tex, frag_texcoord);
    fragColor = vec4(result, 1.0) * tex_color;
}
