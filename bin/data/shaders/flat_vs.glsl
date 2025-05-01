#version 330 core

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

in vec3 position;
in vec3 normal;

flat out vec3 flat_normal;
out vec3 frag_pos;

void main() {
    vec4 view_pos = modelViewMatrix * vec4(position, 1.0);
    frag_pos = view_pos.xyz;
    flat_normal = normalize(normalMatrix * normal);  // flat => pas interpolé
    gl_Position = projectionMatrix * view_pos;
}
