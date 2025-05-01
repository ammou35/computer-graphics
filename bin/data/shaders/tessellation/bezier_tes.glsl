#version 400 core
layout(quads, equal_spacing, ccw) in;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

in vec3 tcPosition[]; // 16 sommets du patch
out vec3 fragNormal;

vec3 bezier(vec3 p0, vec3 p1, vec3 p2, vec3 p3, float t) {
    float u = 1.0 - t;
    return
        u * u * u * p0 +
        3.0 * u * u * t * p1 +
        3.0 * u * t * t * p2 +
        t * t * t * p3;
}

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec3 row[4];
    for (int i = 0; i < 4; ++i) {
        int idx = i * 4;
        row[i] = bezier(
            tcPosition[idx + 0],
            tcPosition[idx + 1],
            tcPosition[idx + 2],
            tcPosition[idx + 3],
            u
        );
    }

    vec3 pos = bezier(row[0], row[1], row[2], row[3], v);
    gl_Position = projectionMatrix * modelViewMatrix * vec4(pos, 1.0);
}
