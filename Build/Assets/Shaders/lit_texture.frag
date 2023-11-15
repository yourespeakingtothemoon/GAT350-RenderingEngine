#version 430
// PIXEL SPACE (called per pixel)

// light types
#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

in vec3 fposition; // will receive interpolated vertex positions for each fragment 
in vec3 fnormal;
in vec2 ftexcoord;

out vec4 ocolor; // this is the pixel we draw to the screen 

// this is bound to channel 0
layout(binding = 0) uniform sampler2D tex;

uniform struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;

	vec2 offset;
	vec2 tiling;
} material;


uniform struct Light 
{
int type;
vec3 position;
vec3 direction;
vec3 color;
float intensity;
float range;
float innerAngle;
float outerAngle;

} light;

uniform vec3 ambientLight;

vec3 ads(in vec3 position, in vec3 normal)
{	
	// AMBIENT lighting component
	vec3 ambient = ambientLight;

	// ATTENUATION
	float attenuation = 1;
	if (light.type != DIRECTIONAL)
	{
		float distanceSqr = dot(light.position - position, light.position - position);
		float rangeSqr = pow(light.range, 2.0);
		attenuation = max(0, 1 - pow((distanceSqr/rangeSqr), 2.0));
		attenuation = pow(attenuation, 2.0);
	}

	// DIFFUSE lighting component based on the light direction and surface normal 
	vec3 lightDir = (light.type == DIRECTIONAL) ?  normalize(-light.direction) : normalize(light.position - position); 
	
	float spotIntensity = 1;
	if (light.type == SPOT)
	{
		float angle = acos(dot(light.direction, -lightDir)); // light.direction = direction light is pointing / lightDir is direction from surface we're lighting to the light 
		//if (angle > light.innerAngle) spotIntensity = 0;
		spotIntensity = smoothstep(light.outerAngle + 0.001, light.innerAngle, angle);
	}

	float intensity = max(dot(lightDir, normal), 0) * spotIntensity;
	vec3 diffuse = material.diffuse * (light.color * intensity) * light.intensity;
	


	// SPECULAR lighting component, contributing only if the surface is facing the light 
	vec3 specular = vec3(0);
	if (intensity > 0) // checks whether the surface is facing the light source 
	{
		vec3 reflection = reflect(-lightDir, normal); // calculate reflection vector (which direction light bounces off surface)
		vec3 viewDir = normalize(-position); // calc view dir vector (normalized vector pointing from frag position to camera)
		float intensity = max(dot(reflection, viewDir), 0); // dot product of reflection vector and view direction (angle between ref vector and view vector)
		intensity = pow(intensity, material.shininess); // raise intensity to power of shininess setting in material 
		specular = material.specular * intensity * spotIntensity * light.intensity * light.color; // final specular color (lightcolor * (material specular color * intensity)
	}
	// returns final light color 
	return ambient + (diffuse + specular) * light.intensity * attenuation;
}

void main()
{
	// samples a color from the texture based on texture coordinates 
	vec4 texcolor = texture(tex, ftexcoord); 

	// mult texture color with final light color, assigns to output color which is written to framebuffer 
	ocolor = texcolor * vec4(ads(fposition, fnormal), 1);

}
	