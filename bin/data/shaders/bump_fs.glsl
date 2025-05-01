#version 330 core

in vec2 vTexCoord;
in vec3 vTangentLightPos;
in vec3 vTangentViewPos;
in vec3 vTangentFragPos;

out vec4 fragColor;

uniform sampler2D uTex;
uniform sampler2D uNormalMap;

void main()
{
    vec3 albedo = texture(uTex, vTexCoord).rgb;
    vec3 normal = texture(uNormalMap, vTexCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0); // de [0,1] à [-1,1]

    vec3 lightDir = normalize(vTangentLightPos - vTangentFragPos);
    vec3 viewDir = normalize(vTangentViewPos - vTangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(lightDir, normal), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 ambient = 0.1 * albedo;
    vec3 diffuse = 0.6 * diff * albedo;
    vec3 specular = 0.3 * spec * vec3(1.0);

    vec3 result = ambient + diffuse + specular;
    fragColor = vec4(result, 1.0);
}
