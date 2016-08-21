#version 330

layout (location = 0) in vec3 Position; // TODO: rename to vertex
layout (location = 1) in vec2 Texture;
layout (location = 2) in vec3 Normal;

uniform mat4 projection;

out vec2 UV;

void main() {	
	UV = Texture;
	gl_Position = projection * vec4(Position, 1.0);
}
