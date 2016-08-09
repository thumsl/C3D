#version 330

layout (location = 0) in vec3 Position; // TODO: rename to vertex
layout (location = 1) in vec2 Texture;
layout (location = 2) in vec3 Normal;

uniform mat4 MVP;
uniform mat4 ModelView;
uniform mat4 Transform;
uniform mat4 Projection;

out vec2 UV;
out vec3 normal;
out vec3 transformedWorld;
out float visibility;

float density = 0.1f;
float gradient = 1.5f;

void main() {
	vec4 camera_position = ModelView * vec4(Position, 1.0);
	float distanceToCamera = -camera_position.z;
	
	UV = Texture;
	normal = Normal;
	transformedWorld = (Transform * vec4(Position, 1.0)).xyz;

	visibility = exp(-pow(distanceToCamera*density, gradient));
	visibility = clamp(visibility, 0.0f, 1.0f);
	gl_Position = Projection * camera_position;
}
