#version 400 core
layout(quads, equal_spacing, ccw) in;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

vec3 bezier(vec3 p0, vec3 p1, vec3 p2, vec3 p3, float t) {
    float u = 1.0 - t;
    return
        u*u*u*p0 +
        3.0*u*u*t*p1 +
        3.0*u*t*t*p2 +
        t*t*t*p3;
}

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // Calcul de la courbe en u (pour chaque ligne)
    vec3 curveU[4];
    for (int i = 0; i < 4; ++i) {
        int idx = i * 4;
        curveU[i] = bezier(
            gl_in[idx + 0].gl_Position.xyz,
            gl_in[idx + 1].gl_Position.xyz,
            gl_in[idx + 2].gl_Position.xyz,
            gl_in[idx + 3].gl_Position.xyz,
            u
        );
    }

    vec3 position = bezier(curveU[0], curveU[1], curveU[2], curveU[3], v);
    gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}
