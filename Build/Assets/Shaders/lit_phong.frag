#version 430

#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

#define ALBEDO_TEXTURE_MASK   1
#define SPECULAR_TEXTURE_MASK 2
#define NORMAL_TEXTURE_MASK   4
#define EMISSIVE_TEXTURE_MASK 8

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 normal;
in layout(location = 2) vec2 texcoord;
in layout(location = 3) vec4 shadowcoord;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D albedoTexture;
layout(binding = 1) uniform sampler2D specularTexture;
layout(binding = 2) uniform sampler2D normalTexture;
layout(binding = 3) uniform sampler2D emissiveTexture;
layout(binding = 5) uniform sampler2D shadowTexture;

uniform struct Material {
	uint params;

	vec3 albedo;
	vec3 specular;
	vec3 emissive;
	float shininess;

	vec2 offset;
	vec2 tiling;
} material;

uniform struct Light {
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
	float range;
	float innerAngle;
	float outerAngle;
} lights[3];

uniform vec3 ambientLight;
uniform int numLights = 3;
uniform float shadowBias = 0.005;

void phong(in Light light, in vec3 position, in vec3 normal, out vec3 diffuse, out vec3 specular) {
	// Diffuse light calculations
	vec3 lightDir = vec3(0);
	if(light.type == DIRECTIONAL) {
		lightDir = normalize(-light.direction);
	} else {
		lightDir = normalize(light.position - position);
	}

	float spotIntensity = 1;
	if(light.type == SPOT) {
		float angle = acos(dot(light.direction, -lightDir));
		spotIntensity = smoothstep(light.outerAngle + 0.001, light.innerAngle, angle);
	}

	float intensity = max(dot(lightDir, normal), 0) * spotIntensity;
	diffuse = (light.color * intensity);

	// Specular light calculations
	specular = vec3(0);
	// Skip calculations if no light is present
	if(intensity > 0) {
		// Phong
		vec3 viewDir = normalize(-position);
		//vec3 reflection = reflect(-lightDir, normal);
		//intensity = max(dot(reflection, viewDir), 0);

		// Blinn-Phong
		vec3 h = normalize(viewDir + lightDir);
		intensity = max(dot(h, normal), 0);
		
		intensity = pow(intensity, material.shininess);
		specular = vec3(intensity * spotIntensity);
	}
}

float attenuation(in vec3 position1, in vec3 position2, in float range) {
	float distanceSqr = dot(position1 - position2, position1 - position2);
	float rangeSqr = pow(range, 2.0);
	float attenuation = max(0, 1 - pow((distanceSqr / rangeSqr), 2.0));
	attenuation = pow(attenuation, 2.0);
 
	return attenuation;
}

float calculateShadow(in vec4 shadowcoord, in float bias) {
	return texture(shadowTexture, shadowcoord.xy).x < shadowcoord.z - bias ? 0 : 1;
}

void main()
{
	vec4 albedoColor = bool(material.params & ALBEDO_TEXTURE_MASK) ? texture(albedoTexture, texcoord) : vec4(material.albedo, 1);
	vec4 specularColor = bool(material.params & SPECULAR_TEXTURE_MASK) ? texture(specularTexture, texcoord) : vec4(material.specular, 1);
	vec4 emissiveColor = bool(material.params & EMISSIVE_TEXTURE_MASK) ? texture(emissiveTexture, texcoord) : vec4(material.emissive, 1);

	// Multiply texture color by ambient lighting
	ocolor = vec4(ambientLight, 1) * albedoColor + emissiveColor;

	float shadow = calculateShadow(shadowcoord, shadowBias);

	for(int i = 0; i < numLights; i++) {
		vec3 diffuse;
		vec3 specular;

		float attenuation = (lights[i].type == DIRECTIONAL) ? 1 : attenuation(lights[i].position, position, lights[i].range);

		phong(lights[i], position, normal, diffuse, specular);
		ocolor += ((vec4(diffuse, 1) * albedoColor) + (vec4(specular, 1)) * specularColor) * attenuation * lights[i].intensity * shadow;
	}
}
