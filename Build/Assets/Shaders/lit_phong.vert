#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec2 vtexcoord;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec2 otexcoord;
out layout(location = 2) vec3 onormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform struct Material
{
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
	mat4 modelView = view * model;

	//opoisition found in world space
	oposition = vec3(modelView * vec4(vposition, 1));
	//onormal found in world space
	onormal = normalize(mat3(modelView) * vnormal);

	//texcoords are scaled and offset
	otexcoord = (vtexcoord * material.tiling) + material.offset;

	//render the asset in world-view 
	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vposition, 1.0);
}
