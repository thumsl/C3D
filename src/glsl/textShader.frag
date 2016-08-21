#version 330

out vec4 color;

in vec2 UV;

uniform sampler2D sampler;
uniform vec2 offset;

void main() {
	color = texture2D(sampler, UV + offset);
}