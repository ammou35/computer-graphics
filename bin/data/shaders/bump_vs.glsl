#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 6) in vec3 tangent;

out vec2 vTexCoord;
out vec3 vTangentLightPos;
out vec3 vTangentViewPos;
out vec3 vTangentFragPos;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

uniform vec3 uLightPos;
uniform vec3 uViewPos;

void main()
{
    vec3 T = normalize(normalMatrix * tangent);
    vec3 N = normalize(normalMatrix * normal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T, B, N);

    vec4 worldPos = modelViewMatrix * vec4(position, 1.0);
    vec3 fragPos = worldPos.xyz;

    vTangentFragPos = TBN * fragPos;
    vTangentLightPos = TBN * uLightPos;
    vTangentViewPos = TBN * uViewPos;

    vTexCoord = texcoord;
    gl_Position = projectionMatrix * worldPos;
}
