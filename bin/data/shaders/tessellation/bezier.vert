#version 400 core
layout(location = 0) in vec3 position;

out vec3 vPosition; // transmis au TCS

void main() {
    vPosition = position;
}
