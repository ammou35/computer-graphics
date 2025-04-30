#version 330

in vec3 surface_normal;
in vec3 surface_position;
in vec2 surface_texcoord;

uniform vec3 light_position;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float brightness;

uniform sampler2D tex;

out vec4 fragment_color;

void main()
{
    vec3 n = normalize(surface_normal);
    vec3 l = normalize(light_position - surface_position);
    float reflection_diffuse = max(dot(n, l), 0.0);

    float reflection_specular = 0.0;
    if (reflection_diffuse > 0.0)
    {
        vec3 v = normalize(-surface_position);
        vec3 h = normalize(v + l);
        reflection_specular = pow(max(dot(n, h), 0.0), brightness);
    }

    vec3 tex_color = texture(tex, surface_texcoord).rgb;

    fragment_color = vec4(
        color_ambient * tex_color +
        color_diffuse * tex_color * reflection_diffuse +
        color_specular * reflection_specular,
        1.0
    );
}