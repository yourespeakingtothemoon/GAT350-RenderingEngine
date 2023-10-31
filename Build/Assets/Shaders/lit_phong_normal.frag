#version 430

#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

#define MAX_LIGHTS 3

#define ALBEDO_TEXTURE_MASK		(1 << 0) //0001
#define SPECULAR_TEXTURE_MASK	(1 << 1) //0010
#define NORMAL_TEXTURE_MASK		(1 << 2) //0100
#define EMISSIVE_TEXTURE_MASK	(1 << 3) //1000

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec2 ftexcoord;
in layout(location = 2) mat3 ftbn;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D albedoTexture;
layout(binding = 1) uniform sampler2D specularTexture;
layout(binding = 2) uniform sampler2D normalTexture;
layout(binding = 3) uniform sampler2D emissiveTexture;

uniform float time;

//imgui editable variables
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


uniform struct Light
{
	int type;
	vec3 position;
	vec3 direction;
	vec3 color;
//	float cutoff;
	float intensity;
	float innerAngle;
float outerAngle;
float range;
} lights[MAX_LIGHTS];

uniform vec3 ambientLight;

void phong(in Light light, in vec3 position, in vec3 normal, out vec3 diffuse, out vec3 specular)
{
	//Diffuse
		//get light direciton by normalizing the light position - the position of the object
	vec3 lightDir = (light.type == DIRECTIONAL) ? normalize(light.direction) : (light.position - position);
		//get light intensity factor by getting the dot product of the light direction and the surface normal and then maximizing
	float spotIntensity =1;
	if(light.type == SPOT)
	{
	float cosine = acos(dot(light.direction, -lightDir));
	spotIntensity = smoothstep(light.outerAngle+0.001, light.innerAngle, cosine);

	//if(cosine > light.cutoff){ spotIntensity = 0; } else{ spotIntensity = pow(cos(cosine), light.cutoff);}
	//spotIntensity = dot(normalize(light.direction), -lightDir);
	}


	float intensity = max(dot(lightDir, normal), 0) * spotIntensity;
		//take the material diffuse value and multiply it by our color and calculated intensity
	diffuse = (light.color * intensity); //* light.intensity;
	//Specular
	specular = vec3(0);
	if(intensity > 0)
	{
		//reflection vector found by reflecting the light direction and the surface normal
		vec3 reflection = reflect(-lightDir, normal);
		//view direction (specular lighting is view depenant) found by normalizing the position of the object
		vec3 viewDir = normalize(-position);
		//specular intensity is found by maximizing the dot product of previously found specular variables
		intensity = max(dot(reflection, viewDir), 0);
		//adjust the intensity with the shininess factor
		intensity = pow(intensity, material.shininess);
		//calculate final light
		specular = vec3(intensity * spotIntensity);
	}


};



//vec3 ads( vec3 position, vec3 normal)
//{
////Lighting
//	//Ambient - constant color 
//	vec3 ambient = ambientLight;
//	//Attneuation
//	float attenuation = 1;
//if(light.type != DIRECTIONAL)
//	{
//float dSqr = dot(light.position - position, light.position - position);
//float rSqr = light.range * light.range;
//float att = max(0,1 - pow((dSqr / rSqr),2));
//attenuation = att * att;
//	
//	}
//
//
//	//return all lighting elements
//	return ambient + (diffuse + specular) *light.intensity * attenuation;
//};

void main()
{

vec4 albedoColor = bool(material.params & ALBEDO_TEXTURE_MASK) ? texture(albedoTexture, ftexcoord) : vec4(material.albedo,1);
vec4 specularColor = bool(material.params & SPECULAR_TEXTURE_MASK) ? texture(specularTexture, ftexcoord) : vec4(material.specular,1);
vec4 emissiveColor = bool(material.params & EMISSIVE_TEXTURE_MASK) ? texture(emissiveTexture, ftexcoord) : vec4(material.emissive,1);


//modulate texcolor by light
	vec4 texcolor = texture(albedoTexture, ftexcoord);
	

	ocolor = vec4(ambientLight,1) * albedoColor + emissiveColor; 

	for(int i =0; i < MAX_LIGHTS; i++)
	{
	vec3 diffuse;
	vec3 specular;
	float attenuation = (lights[i].type == DIRECTIONAL) ? 1 : max(0,1 - pow((dot(lights[i].position - fposition, lights[i].position - fposition) / (lights[i].range * lights[i].range)),2));

	vec3 normal=texture(normalTexture, ftexcoord).rgb;
	normal = (normal * 2) -1;
	normal = normalize(ftbn*normal);

	phong (lights[i] , fposition, normal, diffuse, specular);
ocolor += (vec4(diffuse, 1)*albedoColor) + (vec4(specular,1)*specularColor) *lights[i].intensity * attenuation;
	}
	// * vec4(ads(fposition, fnormal), 1);
}
