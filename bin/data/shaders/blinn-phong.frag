#version 330
in vec3 vNormal;
in vec3 vPosition;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
out vec4 fragColor;

void main() {
    vec3 N = normalize(vNormal);
    vec3 L = normalize(lightPosition - vPosition);
    vec3 V = normalize(viewPosition - vPosition);
    vec3 H = normalize(L + V); // Halfway vector

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(N, H), 0.0), 64.0);

    vec3 color = vec3(0.2) + vec3(0.7) * diff + vec3(1.0) * spec;
    fragColor = vec4(color, 1.0);
}
