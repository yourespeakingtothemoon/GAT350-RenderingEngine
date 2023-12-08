#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec2 texcoord;

out layout(location = 0) vec2 otexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform struct Material {
	vec3 albedo;

	vec2 offset;
	vec2 tiling;
} material;

void main()
{
	otexcoord = (texcoord * material.tiling) + material.offset;

	// Get world position
	vec3 pos = model[3].xyz;
	// Get size
	vec2 size = vec2(length(model[0]), length(model[1]));
	// Get view right direction
	vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
	// Get world up direction
	vec3 up = vec3(0, 1, 0);

	// Set (local) vertex position using world position and view direction
	pos = pos + (position.x * right * size.x) + (position.y * up * size.y);

	mat4 vp = projection * view;
	gl_Position = vp * vec4(pos, 1.0);
}
