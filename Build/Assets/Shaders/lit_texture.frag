#version 430

#define POINT 0
#define DIRECTIONAL 1
#define SPOT 2

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec2 ftexcoord;
in layout(location = 2) vec3 fnormal;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D tex;

uniform float time;

//imgui editable variables
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
	float cutoff;
} light;

uniform vec3 ambientLight;

vec3 ads( vec3 position, vec3 normal)
{
//Lighting
	//Ambient - constant color 
	vec3 ambient = ambientLight;
	//Diffuse
		//get light direciton by normalizing the light position - the position of the object
	vec3 lightDir = (light.type == DIRECTIONAL) ? normalize(light.direction) : (light.position - position);
		//get light intensity factor by getting the dot product of the light direction and the surface normal and then maximizing
	float spotIntensity =1;
	if(light.type == SPOT)
	{
	float cosine = acos(dot(light.direction, -lightDir));
	if(cosine > light.cutoff){ spotIntensity = 0; } else{ spotIntensity = pow(cos(cosine), light.cutoff);}
	//spotIntensity = dot(normalize(light.direction), -lightDir);
	}


	float intensity = max(dot(lightDir, normal), 0);
		//take the material diffuse value and multiply it by our color and calculated intensity
	vec3 diffuse = material.diffuse * (light.color * intensity * spotIntensity);
	//Specular
	vec3 specular = vec3(0);
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
		specular = material.specular * intensity;
	}
	//return all lighting elements
	return ambient + diffuse + specular;
};

void main()
{
//modulate texcolor by light
	vec4 texcolor = texture(tex, ftexcoord);
	ocolor = texcolor * vec4(ads(fposition, fnormal), 1);
}
