#version 330

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec3 surface_position;
out vec3 surface_normal;
out vec2 surface_texcoord;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main()
{
    surface_position = vec3(modelViewMatrix * vec4(position, 1.0));
    surface_normal = normalize(normalMatrix * normal);
    surface_texcoord = texcoord;

    gl_Position = projectionMatrix * vec4(surface_position, 1.0);
}
