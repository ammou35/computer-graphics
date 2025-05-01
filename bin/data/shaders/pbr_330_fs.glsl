#version 330 core

#define MAX_LIGHTS 30

in vec3 frag_pos;
in vec3 frag_normal;
in vec2 frag_texcoord;

uniform sampler2D tex0;
uniform sampler2D normalMap;

uniform int useNormalMap;
uniform float material_roughness;
uniform float material_metallic;
uniform float brightness;

uniform vec3 color_diffuse;
uniform vec3 color_specular;
uniform vec3 color_ambient;

uniform vec3 light_positions[MAX_LIGHTS];
uniform vec3 light_colors[MAX_LIGHTS];
uniform vec3 light_directions[MAX_LIGHTS];
uniform int light_type[MAX_LIGHTS];  // 0: point, 1: dir, 2: spot
uniform float spot_cutoffs[MAX_LIGHTS]; // cos(theta)
uniform int num_lights;
uniform vec3 light_ambient;

out vec4 fragColor;

// PBR utils
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float denom = (NdotH * NdotH * (a2 - 1.0) + 1.0);
    return a2 / (3.14159265 * denom * denom);
}
float geometrySchlickGGX(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    return geometrySchlickGGX(max(dot(N, V), 0.0), roughness) *
           geometrySchlickGGX(max(dot(N, L), 0.0), roughness);
}

void main() {
    vec3 albedo = texture(tex0, frag_texcoord).rgb * color_diffuse;
    vec3 N = normalize(frag_normal);
    vec3 V = normalize(-frag_pos);

    if (useNormalMap == 1) {
        vec3 normalTex = texture(normalMap, frag_texcoord).rgb;
        normalTex = normalize(normalTex * 2.0 - 1.0);
        N = normalTex;
    }

    vec3 F0 = mix(vec3(0.04), albedo, material_metallic);
    vec3 Lo = vec3(0.0);

    for (int i = 0; i < num_lights; ++i) {
        vec3 L;
        float attenuation = 1.0;

        if (light_type[i] == 1) {
            // Directional light
            L = normalize(-light_directions[i]);
        } else {
            // Point or spot light
            vec3 light_vec = light_positions[i] - frag_pos;
            float dist = length(light_vec);
            L = normalize(light_vec);

            // ⛔ SPOT CUT OFF (Blinn-Phong style)
            if (light_type[i] == 2) {
                float spotEffect = dot(-light_directions[i], normalize(light_vec));
                if (spotEffect < spot_cutoffs[i]) continue; // trop loin du cône
            }

            // Atténuation douce
            float linear = 0.001;
            float quadratic = 0.0001;
            attenuation = 1.0 / (1.0 + linear * dist + quadratic * dist * dist);
        }

        vec3 H = normalize(V + L);
        vec3 radiance = light_colors[i] * brightness * attenuation;

        float D = distributionGGX(N, H, material_roughness);
        float G = geometrySmith(N, V, L, material_roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = (1.0 - kS) * (1.0 - material_metallic);

        float NdotL = max(dot(N, L), 0.0);
        vec3 specular = (D * G * F) / max(4.0 * max(dot(N, V), 0.0) * NdotL, 0.001);

        Lo += (kD * albedo / 3.14159265 + specular) * radiance * NdotL;
    }

    vec3 ambient = light_ambient * albedo;
    vec3 color = ambient + Lo;
    color = pow(color, vec3(1.0 / 2.2));
    fragColor = vec4(color, 1.0);
}
