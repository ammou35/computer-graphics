#version 330

in vec2 v_texcoord;
uniform sampler2D tex0;

out vec4 fragColor;

void main() {
    vec4 texColor = texture(tex0, v_texcoord);
    fragColor = vec4(vec3(1.0) - texColor.rgb, texColor.a);
}
