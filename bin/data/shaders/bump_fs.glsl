#version 330 core

#define MAX_LIGHTS 8

in vec2 vTexCoord;
in vec3 vFragPos;
in mat3 TBN;

uniform sampler2D uTex;
uniform sampler2D uNormalMap;
uniform vec3 uViewPos;

uniform int numLights;
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];
uniform vec3 lightDirections[MAX_LIGHTS];
uniform int lightTypes[MAX_LIGHTS]; // 0 = point, 1 = directional, 2 = spot
uniform float spotCutoffs[MAX_LIGHTS];

out vec4 fragColor;

void main() {
    vec3 color = texture(uTex, vTexCoord).rgb;
    vec3 normalTex = texture(uNormalMap, vTexCoord).rgb;
    vec3 normal = normalize(normalTex * 2.0 - 1.0);
    vec3 N = normalize(normal);

    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 result = 0.1 * color;  // lumière ambiante

    for (int i = 0; i < numLights; ++i) {
        vec3 L;

        if (lightTypes[i] == 0) {
            L = normalize(lightPositions[i] - vFragPos); // point light
        } else if (lightTypes[i] == 1) {
            L = normalize(-lightDirections[i]); // directional light
        } else if (lightTypes[i] == 2) {
	    vec3 lightToFrag = normalize(vFragPos - lightPositions[i]); // vecteur de la lumière vers le fragment
	    float theta = dot(normalize(-lightDirections[i]), lightToFrag); // comparaison angle
	    if (theta < spotCutoffs[i]) continue;
	    L = -lightToFrag; // on éclaire depuis la lumière vers le fragment
	}

        float diff = max(dot(N, L), 0.0);
        vec3 reflectDir = reflect(-L, N);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);

        vec3 diffuse = diff * lightColors[i];
        vec3 specular = spec * vec3(1.0);

        result += (diffuse + specular) * color;
    }

    fragColor = vec4(result, 1.0);
}
