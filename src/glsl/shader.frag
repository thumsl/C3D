#version 330

in vec2 UV;

out vec4 color;
uniform sampler2D sampler;

void main() {
    color = texture2D(sampler, UV);
}
