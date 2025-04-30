#version 330 core

uniform mat4 modelViewMatrix; // ← we treat this as world-space model matrix
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

in vec4 position;
in vec3 normal;
in vec2 texcoord;

out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_texcoord;

void main()
{
    frag_pos = vec3(modelViewMatrix * position); // ← pretend it's world-space
    frag_normal = normalize(normalMatrix * normal);
    frag_texcoord = texcoord;

    gl_Position = projectionMatrix * modelViewMatrix * position;
}
