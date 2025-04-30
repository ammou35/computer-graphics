#version 330

in vec2 v_texcoord;
uniform sampler2D tex0;

out vec4 fragColor;

void main() {
    vec4 col = vec4(0.0);
    float offset = 1.0 / 512.0; // ajuster selon résolution texture

    // 3x3 kernel blur
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            vec2 samplePos = v_texcoord + vec2(x, y) * offset;
            col += texture(tex0, samplePos);
        }
    }
    col /= 9.0;
    fragColor = col;
}
