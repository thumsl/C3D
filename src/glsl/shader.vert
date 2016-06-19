#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 Texture;
layout (location = 2) in vec3 Normal;

uniform mat4 MVP;

out vec2 UV;
out vec3 normal;

void main() {
	gl_Position = MVP * vec4(Position, 1.0);

	UV = Texture;
	normal = Normal;
}
