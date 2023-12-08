#version 430

in layout(location = 0) vec3 position;
in layout(location = 2) vec3 normal;

out layout(location = 0) vec3 otexcoord;

uniform float ior = 1.55;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	// Transform vertex position and normal to world space
	vec3 pos = vec3(model * vec4(position, 1));
	vec3 nor = normalize(mat3(model) * normal);

	// Inverse of view space -> world space
	// Last column of mat4 is translation
	vec3 viewPosition = inverse(view)[3].xyz;
	vec3 viewDir = normalize(pos - viewPosition);

	// Refract view direction about vertex normal using ior
	otexcoord = refract(viewDir, nor, 1.0 / ior);

	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(position, 1.0);
}
