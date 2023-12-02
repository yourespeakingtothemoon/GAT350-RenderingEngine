#version 430
#define POINT       0
#define DIRECTIONAL 1
#define SPOT        2

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D tex;

uniform struct Material {
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec2 offset;
	vec2 tiling;
} material;

uniform struct Light {
	int type;
	vec3 ambientLight;
	vec3 diffuseLight;
	vec3 lightPosition;
	vec3 direction;
	float innerAngle;
	float outerAngle;
	float intensity;
	float range;
} light;

vec3 ads(in vec3 position, in vec3 normal)
{
	// AMBIENT
	vec3 ambient = light.ambientLight;

	// Attenuation
   float attenuation = 1.0;
   if (light.type != DIRECTIONAL)
   {
       vec3 toLight = light.lightPosition - fposition;
       float distanceSqr = dot(toLight, toLight);
       float rangeSqr = pow(light.range, 2.0);
       attenuation = max(0.0, 1.0 - distanceSqr / rangeSqr);
       attenuation = pow(attenuation, 2.0);
   }

	// DIFFUSE
	vec3 lightDir = (light.type == DIRECTIONAL) ? normalize(-light.direction) : normalize(light.lightPosition - position);

	float spotIntensity = 1;
	if (light.type == SPOT)
	{
		float angle = acos(dot(light.direction, -lightDir));
		spotIntensity = smoothstep(light.outerAngle + 0.001f, light.innerAngle, angle);
	}

	float intensity = max(dot(lightDir, normal), 0) * spotIntensity;
	vec3 diffuse = material.diffuse * (light.diffuseLight * intensity);

	// SPECULAR
	vec3 specular = vec3(0);
	if (intensity > 0)
	{
		vec3 reflection = reflect(-lightDir, normal);
		vec3 viewDir = normalize(-position);
		//vec3 halfwayDir = normalize(lightDir + viewDir);
		intensity = max(dot(reflection, viewDir), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity * spotIntensity;
	}

	return ambient + (diffuse + specular) * light.intensity * attenuation;
}

void main()
{
	vec4 texcolor = texture(tex, ftexcoord);
	ocolor = texcolor * vec4(ads(fposition, fnormal), 1);
}