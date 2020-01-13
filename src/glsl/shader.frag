#version 330

out vec4 color;

in vec2 UV;
in vec3 normal;
in vec3 transformedWorld;
in float visibility;

struct ambientLight {
	vec3 color;
	float intensity;
};

struct pointLight {
    vec3 position;
    vec3 color;
    float attenuation;
    float intensity;
};

uniform sampler2D sampler;
uniform ambientLight ambient;
uniform pointLight point;
uniform float specularPower;
uniform float specularIntensity;

uniform vec3 skyColor;
uniform vec3 eyePos;

void main() {
	// Ambient
	vec4 ambientColor = vec4(ambient.color * ambient.intensity, 1.0f);
	vec4 specularColor = vec4(0,0,0,0);
	vec4 diffuseColor = vec4(0,0,0,0);

	// Diffuse
	vec3 lightDirection = -normalize(transformedWorld - point.position);
	float diffuseFactor = dot(normalize(normal), lightDirection);

	if (diffuseFactor > 0) {
		float distance = length(transformedWorld - point.position);
		float attenuation = 1.0f / (1.0f + point.attenuation * distance + point.attenuation * distance * distance);
		diffuseColor = vec4(point.color * diffuseFactor * attenuation * point.intensity, 1.0f);

		// Specular
		vec3 vertexToEye = normalize(eyePos - transformedWorld);
		vec3 LightReflect = normalize(reflect(-lightDirection, normal));
		float SpecularFactor = dot(vertexToEye, LightReflect);
		if (SpecularFactor > 0) {
			SpecularFactor = pow(SpecularFactor, specularPower);
		    specularColor = vec4(point.color * specularIntensity * SpecularFactor * attenuation, 1.0f);
		}
	}

	// Phong
	color = texture(sampler, UV) * (ambientColor +  diffuseColor + specularColor);
	color = mix(vec4(skyColor, 1.0), color, visibility);
}
