#version 330
uniform sampler2D image;
in vec2 surface_texcoord;
out vec4 outputColor;

void main()
{
    vec4 color = texture(image, surface_texcoord);
    // Effet chaud avec dominance rouge/jaune
    vec3 mex = color.rgb * vec3(1.2, 0.9, 0.6);
    mex.r = min(mex.r * 1.3, 1.0);
    mex.b *= 0.7;
    outputColor = vec4(mex, color.a);
}