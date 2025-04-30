#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoord;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 frag_pos;
out vec3 frag_normal;
out vec2 frag_texcoord;

void main()
{
    vec4 view_pos = modelViewMatrix * vec4(position, 1.0);
    frag_pos = view_pos.xyz;
    frag_normal = normalize(normalMatrix * normal);
    frag_texcoord = texcoord;

    gl_Position = projectionMatrix * view_pos;
}
