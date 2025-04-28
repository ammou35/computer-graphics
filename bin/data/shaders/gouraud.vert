#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

out vec4 vertexColor;

void main() {
    vec3 N = normalize(normalMatrix * normal);
    vec3 V = normalize(viewPosition - (modelViewMatrix * vec4(position, 1.0)).xyz);
    vec3 L = normalize(lightPosition - (modelViewMatrix * vec4(position, 1.0)).xyz);
    vec3 H = normalize(L + V);

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), 32.0);

    vec3 color = vec3(0.2) + vec3(0.7) * diff + vec3(1.0) * spec;
    vertexColor = vec4(color, 1.0);

    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);
}