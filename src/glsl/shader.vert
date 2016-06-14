#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 Texture;

uniform mat4 MVP;

out vec2 UV;

void main() {
	gl_Position = MVP * vec4(Position, 1.0);

	UV = Texture;
}
