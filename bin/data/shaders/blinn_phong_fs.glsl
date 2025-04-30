#version 330

in vec3 v_normal;
in vec3 v_position;
in vec2 v_texcoord;

uniform vec3 light_position;

uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform float brightness;

uniform sampler2D tex;

out vec4 fragColor;

void main()
{
    vec3 N = normalize(v_normal);
    vec3 L = normalize(light_position - v_position);
    vec3 V = normalize(-v_position); // vue vers la caméra
    vec3 H = normalize(L + V); // Blinn

    float lambert = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), brightness);

    vec3 texColor = texture(tex, v_texcoord).rgb;

    vec3 ambient = color_ambient * texColor;
    vec3 diffuse = lambert * color_diffuse * texColor;
    vec3 specular = spec * color_specular;

    fragColor = vec4(ambient + diffuse + specular, 1.0);
}
