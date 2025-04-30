#version 330

in vec2 v_texcoord;
uniform sampler2D tex0;

out vec4 fragColor;

void main() {
    vec4 texColor = texture(tex0, v_texcoord);

    // Teinte rouge/orange/jaune, saturation poussée
    vec3 warm = vec3(texColor.r * 1.2, texColor.g * 0.9, texColor.b * 0.7);
    fragColor = vec4(clamp(warm, 0.0, 1.0), texColor.a);
}
