#version 430

//in layout(location = 0) vec3 color;
in layout(location = 1) vec2 texcoord;

out layout(location = 0) vec4 ocolor;

uniform vec4 color;
uniform vec2 offset;
uniform vec2 tiling;

layout(binding = 0) uniform sampler2D tex;

uniform float time;

void main()
{
	vec4 texcolor = texture(tex, (texcoord + offset) * tiling );
	//if(texcolor.a < 0.8) discard;
	ocolor = texcolor * color;
}
