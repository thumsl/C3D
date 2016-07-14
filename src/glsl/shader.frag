#version 330

out vec4 color;

in vec3 position;
in vec2 UV;
in vec3 normal;

struct pointLight
{
        //int type; for a more generic struct
        vec3 position;
        float attenuation;
        vec3 color;
};

uniform sampler2D sampler;
uniform pointLight light;

uniform vec3 ambientLightColor;
uniform float ambientLightIntensity;
uniform float intensity;


void main() {
	vec4 ambient = vec4((ambientLightColor * ambientLightIntensity), 1.0f);

	vec3 lightDirection = position - light.position;
	float dist = length(lightDirection);
	lightDirection = normalize(lightDirection);

	float att = 1.0f / (1.0f + light.attenuation * dist + light.attenuation * dist * dist);
	vec4 point = vec4 ( clamp( (dot(normal, -lightDirection) * light.color) * att, 0.0f, 1.0f) * intensity, 1.0f);

	color = texture2D(sampler, UV) * (ambient + point);
}