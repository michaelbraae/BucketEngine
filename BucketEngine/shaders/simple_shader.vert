#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec2 uv;

layout (location = 0) out vec3 fragColor;

// normal matrix is actually mat3, but we're using 4 here for alignment reasons
layout(push_constant) uniform Push {
    mat4 transform;
    mat4 normalMatrix;
} push;

const float AMBIENT = .02f;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));

void main() {
    gl_Position = push.transform * vec4(position, 1.0);
    
    vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

    // if we have no light intensity, use our ambient value as a minimum
    float lightIntensity = max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), AMBIENT);

    fragColor = lightIntensity * color;
}
