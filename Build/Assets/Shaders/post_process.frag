#version 430

#define INVERT_MASK        1
#define GRAYSCALE_MASK     2
#define COLORTINT_MASK     4
#define SCANLINE_MASK      8
#define GRAIN_MASK        16
#define DEUTERANOPIA_MASK 32

in layout(location = 0) vec2 texcoord;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D screenTex;

uniform float blend = 1;
uniform uint params = 0;
uniform vec3 tint = vec3(0);

vec4 invert(in vec4 color) {
	return vec4(vec3(1) - color.rbg, color.a);
}

vec4 grayscale(in vec4 color) {
	return vec4(vec3((color.r + color.g + color.b) * 0.33333333333333333333333333333333), color.a);
}

vec4 colortint(in vec4 color) {
	return vec4(vec3(color) * tint, color.a);
}

vec4 scanline(in vec4 color) {
	int yPos = int(gl_FragCoord.y);
	
	vec3 overlay = (yPos % 2) == 0 ? vec3(0) : vec3(1);

	return vec4(vec3(color) * overlay, color.a);
}

float random(vec2 st) {
	return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

vec4 grain(in vec4 color) {
	return vec4(vec3(color) * random(gl_FragCoord.xy), color.a);
}

// region Taken from https://www.shadertoy.com/view/XdjXzV
vec3 RGBtoLMS(vec3 color) {
	return vec3(
		dot(vec3(17.8824,   43.5161,  4.11935), color),
		dot(vec3(3.45565,   27.1554,  3.86714), color),
		dot(vec3(0.0299566, 0.184309, 1.46709), color)
	);
}

vec3 LMStoRGB(vec3 color) {
	return vec3(
		dot(vec3(0.0809444479,    -0.130504409,   0.116721066), color),
		dot(vec3(-0.0102485335,    0.0540193266, -0.113614708), color),
		dot(vec3(-0.000365296938, -0.00412161469, 0.693511405), color)
	);
}

// Green color blindness
vec3 Deuteranope(vec3 color) {
    return vec3(
        dot(vec3(1.0,      0.0, 0.0),     color),
        dot(vec3(0.494207, 0.0, 1.24827), color),
        dot(vec3(0.0,      0.0, 1.0),     color)
    );
}
// endregion

vec4 deuteranopia(in vec4 color) {
	return vec4(LMStoRGB(Deuteranope(RGBtoLMS(vec3(color)))), color.a);
}

void main() {
	vec4 basecolor = texture(screenTex, texcoord);
	vec4 post_process = basecolor;

	if(bool(params & INVERT_MASK)) {
		post_process = invert(post_process);
	}
	if(bool(params & GRAYSCALE_MASK)) {
		post_process = grayscale(post_process);
	}
	if(bool(params & COLORTINT_MASK)) {
		post_process = colortint(post_process);
	}
	if(bool(params & SCANLINE_MASK)) {
		post_process = scanline(post_process);
	}
	if(bool(params & GRAIN_MASK)) {
		post_process = grain(post_process);
	}
	if(bool(params & DEUTERANOPIA_MASK)) {
		post_process = deuteranopia(post_process);
	}

	ocolor = mix(basecolor, post_process, blend);
}
