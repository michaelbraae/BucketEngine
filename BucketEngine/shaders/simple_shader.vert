#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout(location = 0) out vec3 fragColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projectionViewMatrix;
    vec3 directionToLight;
} ubo;

// normal matrix is actually mat3, but we're using 4 here for alignment reasons
layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

const float AMBIENT = .02f;

void main() {
    gl_Position = ubo.projectionViewMatrix * push.modelMatrix * vec4(position, 1.0);
    
    vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

    // if we have no light intensity, use our ambient value as a minimum
    float lightIntensity = max(dot(normalWorldSpace, ubo.directionToLight), AMBIENT);

    fragColor = lightIntensity * color;
}
