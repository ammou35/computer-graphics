#version 400 core
layout(vertices = 16) out;

in vec3 vPosition[];     // entrée du VS
out vec3 tcPosition[];   // sortie vers TES

void main() {
    tcPosition[gl_InvocationID] = vPosition[gl_InvocationID];

    if (gl_InvocationID == 0) {
        gl_TessLevelOuter[0] = 10.0;
        gl_TessLevelOuter[1] = 10.0;
        gl_TessLevelOuter[2] = 10.0;
        gl_TessLevelOuter[3] = 10.0;
        gl_TessLevelInner[0] = 10.0;
        gl_TessLevelInner[1] = 10.0;
    }
}
