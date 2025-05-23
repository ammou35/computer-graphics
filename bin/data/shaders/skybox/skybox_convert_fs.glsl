#version 330 core
in vec3 localPos;
out vec4 FragColor;
uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 sampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main() {
    vec3 dir = normalize(localPos);
    vec2 uv = sampleSphericalMap(dir);
    FragColor = vec4(texture(equirectangularMap, uv).rgb, 1.0);
}
