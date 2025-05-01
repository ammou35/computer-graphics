#version 330

in vec3 vertex_color;
in vec2 surface_texcoord;

out vec4 fragment_color;

uniform sampler2D tex0;

void main() {
    vec3 tex_color = texture(tex0, surface_texcoord).rgb;
    fragment_color = vec4(tex_color * vertex_color, 1.0);
}
