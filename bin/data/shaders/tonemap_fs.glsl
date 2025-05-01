#version 330 core

in vec2 vTexCoord;
out vec4 fragColor;

uniform sampler2D tex0;
uniform float exposure;

void main() {
    vec3 hdrColor = texture(tex0, vTexCoord).rgb;

    // Tonal mapping exponentiel simple
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);

    fragColor = vec4(mapped, 1.0);
}
