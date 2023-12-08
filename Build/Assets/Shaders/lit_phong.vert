#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec2 texcoord;
in layout(location = 2) vec3 normal;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec2 otexcoord;
out layout(location = 3) vec4 oshadowcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 shadowVP;

uniform struct Material {
	uint params;

	vec3 albedo;
	vec3 specular;
	vec3 emissive;
	float shininess;

	vec2 offset;
	vec2 tiling;
} material;

void main()
{
	// Calculate world-view space transform
	mat4 modelView = view * model;

	// Convert position and normal to world-view space
	oposition = vec3(modelView * vec4(position, 1));
	onormal = normalize(mat3(modelView) * normal);

	otexcoord = (texcoord * material.tiling) + material.offset;

	oshadowcoord = shadowVP * model * vec4(position, 1);

	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(position, 1.0);
}
