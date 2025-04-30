#version 330

in vec2 v_texcoord;
uniform sampler2D tex0;

out vec4 fragColor;

void main() {
    vec4 texColor = texture(tex0, v_texcoord);
    float luminance = dot(texColor.rgb, vec3(0.299, 0.587, 0.114)); // standard
    fragColor = vec4(vec3(luminance), texColor.a);
}
