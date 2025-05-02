#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;
layout(location = 6) in vec3 tangent;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec2 vTexCoord;
out vec3 vFragPos;
out mat3 TBN;

void main() {
    vTexCoord = texcoord;

    vec3 T = normalize(normalMatrix * tangent);
    vec3 N = normalize(normalMatrix * normal);
    vec3 B = normalize(cross(N, T));  // Bitangent from N x T

    TBN = mat3(T, B, N);

    vec4 viewPos = modelViewMatrix * vec4(position, 1.0);
    vFragPos = viewPos.xyz;

    gl_Position = projectionMatrix * viewPos;
}
