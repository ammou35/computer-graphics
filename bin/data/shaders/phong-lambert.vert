#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

out vec3 vNormal;
out vec3 vPosition;

void main()
{
    vNormal = normalize(normalMatrix * normal);   // Correctly transformed normal
    vPosition = vec3(modelViewMatrix * vec4(position, 1.0)); // Position in eye space
    gl_Position = modelViewProjectionMatrix * vec4(position, 1.0); // Final screen position
}
