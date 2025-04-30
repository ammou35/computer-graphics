#version 330

uniform sampler2D image;
in vec2 surface_texcoord;
out vec4 outputColor;

void main()
{
    outputColor = texture(image, surface_texcoord);
}