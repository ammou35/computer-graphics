#version 330
in vec3 vNormal;
in vec3 vPosition;
uniform vec3 lightPosition;
out vec4 fragColor;

void main() {
    vec3 lightDir = normalize(lightPosition - vPosition);
    float diff = max(dot(vNormal, lightDir), 0.0);
    fragColor = vec4(vec3(diff), 1.0); // simple gray shading based on diffuse
}