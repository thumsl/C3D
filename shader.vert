#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

out vec3 color;

uniform mat4 transform;
uniform float scale;

void main() {
	color = Color;
    gl_Position = vec4(Position.x * scale, Position.y, Position.z, 1.0) * transform;
}
