#version 330

out vec4 color;

in vec2 UV;
in vec3 normal;

uniform sampler2D sampler;
uniform vec3 diffuseLightDirection;
uniform vec3 diffuseLightColor;

void main() {
	float lightIntensity = clamp(dot(normal, diffuseLightDirection), 0.0f, 1.0f);
	color = texture2D(sampler, UV) * vec4(clamp((diffuseLightColor * lightIntensity), 0.0f, 1.0f), 1.0f);
	
	// float lightIntensity = dot(normal, diffuseLightDirection);

	// if (lightIntensity > 0) {
	// 	color = texture2D(sampler, UV) * vec4(diffuseLightColor * lightIntensity, 1.0f);
	// }
	// else
	// 	color = vec4(0, 0, 0, 0);
    
    //color = texture2D(sampler, UV);
}
