#version 330

out vec4 color;

in vec2 UV;
in vec3 normal;

uniform sampler2D sampler;
uniform vec3 lightDirection;
uniform vec3 lightColor;

void main() {
	float lightIntensity = clamp(dot(normal, lightDirection), 0.0f, 1.0f);

    color = texture2D(sampler, UV) * vec4(clamp((lightColor * lightIntensity), 0.0f, 1.0f), 1.0f);
}
