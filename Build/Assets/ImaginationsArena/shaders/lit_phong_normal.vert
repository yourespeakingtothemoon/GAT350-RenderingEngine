#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec2 texcoord;
in layout(location = 2) vec3 normal;
in layout(location = 3) vec3 tangent;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec2 otexcoord;
out layout(location = 2) vec4 oshadowcoord;
out layout(location = 3) mat3 otbn;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Get the shadowVP matrix from the c++ code
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
	otexcoord = (texcoord * material.tiling) + material.offset;

	// Calculate world-view space transform
	mat4 modelView = view * model;

	// Convert position and normal to world-view space
	oposition = vec3(modelView * vec4(position, 1));

	// Calculate TBN matrix
	vec3 lnormal = normalize(mat3(modelView) * normal);
	vec3 ltangent = normalize(mat3(modelView) * tangent);
	vec3 bitangent = cross(normal, tangent);

	otbn = mat3(ltangent, bitangent, lnormal);

	// Move the shadowVP matrix into model space, then multiply by the position in order to get the shadow coordinates for use in the frag shader
	oshadowcoord = shadowVP * model * vec4(position, 1);

	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(position, 1.0);
}
