#version 330

in vec2 v_texcoord;
uniform sampler2D tex0;

out vec4 fragColor;

void main() {
    vec4 texColor = texture(tex0, v_texcoord);

    // Coordonnées centrées dans [0,1]
    vec2 centeredCoord = v_texcoord - vec2(0.5);
    float dist = length(centeredCoord) * 2.0; // √(x²+y²)
    float vignette = smoothstep(1.0, 0.5, dist); // valeur décroissante vers les bords

    fragColor = vec4(texColor.rgb * vignette, texColor.a);
}
