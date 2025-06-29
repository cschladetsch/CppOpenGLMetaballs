#version 330 core

layout(location = 0) in vec2 position;

out vec2 fragCoord;

void main() {
    fragCoord = position;
    gl_Position = vec4(position * 2.0 - 1.0, 0.0, 1.0);
}