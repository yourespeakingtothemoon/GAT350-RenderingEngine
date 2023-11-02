#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 2) vec3 vnormal;

out layout(location = 0) vec3 otexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float ior = 1.5;


vec3 vreflect(vec3 i,vec3 n)
{
 
return i- (n*dot(n,i)) *2;

}

void main()
{
//transform vertex position/normal to world space
vec3 position = vec3(model * vec4(vposition,1));
vec3 normal = vec3(mat3(model) * vnormal);

vec3 viewPosition = inverse(view)[3].xyz;

vec3 viewDirection = normalize(position - viewPosition);

otexcoord = refract(viewDirection, normal, 1.0/ior);


mat4 mvp = projection * view * model;
gl_Position = mvp * vec4(vposition, 1.0);
}
