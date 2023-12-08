#version 430

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 shadowVP;

void main() {
    gl_Position = shadowVP * model * vec4(position, 1.0);
}
