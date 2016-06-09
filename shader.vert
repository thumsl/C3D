#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

out vec3 color;

void main() {
	color = Color;
    gl_Position = vec4(Position, 1.0);
}