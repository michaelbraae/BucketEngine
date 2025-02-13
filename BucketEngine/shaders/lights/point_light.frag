#version 450

layout (location = 0) in vec2 fragOffset;
layout (location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUbo {
    mat4 projection;
    mat4 view;
    vec4 ambientLightColor;
    vec3 lightPosition;
    vec4 lightColor;
} ubo;

void main() {
    float distance = sqrt(dot(fragOffset, fragOffset));
    if (distance >= 1.0) {
        // discard is a special word in fragment shaders
        // it tells the pipeline to throw away this fragment and return
        discard;
    }

    outColor = vec4(ubo.lightColor.xyz, 1.0);
}
