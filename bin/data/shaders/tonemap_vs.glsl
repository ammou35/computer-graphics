#version 330 core

in vec3 position;
in vec2 texcoord;

out vec2 vTexCoord;

void main() {
    vTexCoord = texcoord;
    gl_Position = vec4(position, 1.0); // NDC, pas besoin de matrices
}
