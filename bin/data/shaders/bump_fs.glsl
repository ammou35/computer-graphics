#version 330 core

in vec2 vTexCoord;
in vec3 vFragPos;
in mat3 TBN;

uniform sampler2D uTex;
uniform sampler2D uNormalMap;
uniform vec3 uLightPos;
uniform vec3 uViewPos;

out vec4 fragColor;

void main() {
    // Sample albedo texture
    vec3 color = texture(uTex, vTexCoord).rgb;

    // Sample and decode normal from normal map
    vec3 normalMap = texture(uNormalMap, vTexCoord).rgb;
    vec3 normal = normalize(normalMap * 2.0 - 1.0); // from [0,1] to [-1,1]

    // Transform normal to view space
    vec3 N = normalize(TBN * normal);

    // Lighting vectors
    vec3 lightDir = normalize(uLightPos - vFragPos);
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, N);

    // Simple Phong
    float diff = max(dot(N, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 diffuse = diff * vec3(1.0);
    vec3 specular = spec * vec3(1.0);

    vec3 result = (diffuse + specular) * color;

    fragColor = vec4(result, 1.0);
}
