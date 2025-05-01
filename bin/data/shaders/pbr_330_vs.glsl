#version 330 core

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec3 frag_pos;     // en VIEW space
out vec3 frag_normal;  // en VIEW space
out vec2 frag_texcoord;

void main() {
    frag_pos = vec3(modelViewMatrix * vec4(position, 1.0));
    frag_normal = normalize(normalMatrix * normal);
    frag_texcoord = texcoord;
    gl_Position = projectionMatrix * vec4(frag_pos, 1.0);
}
