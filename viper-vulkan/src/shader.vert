#version 450

// "i_" meaning input.
// "o_" meaning output.

layout (location = 0) in vec3 i_position; 
layout (location = 1) in vec4 i_color;

layout (location = 0) out vec4 o_color;

layout(binding = 0) uniform UniformBufferObject {
    mat4 projection;
} ubo;

void main() {
    gl_Position = ubo.projection * vec4(i_position, 1.0);
    o_color = i_color;
}
