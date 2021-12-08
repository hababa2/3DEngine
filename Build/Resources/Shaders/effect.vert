#version 430 core 

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} vs_out;

struct UV
{
	vec2 tiling;
	vec2 offset;
};

uniform UV uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

uniform float strength;
uniform float radius;

float random(float n)
{
    return fract(sin(n) * 43758.5453123);
}

float noise(float p)
{
    float fl = floor(p);
    float fc = fract(p);
    return mix(random(fl), random(fl + 1.0), fc);
}

void main()
{
	mat4 model_view = view * model;
	mat3 normal_matrix = transpose(inverse(mat3(model_view)));

	vec3 tPos = position;

	//random
	//tPos *= random(time + tPos.x);

	//sine wave
	//tPos *= (sin(time * 5 + tPos.y) + 2) * 0.2;

	//noise
	//tPos *= noise(time * 5 + tPos.z) + 1;

	// bloat
    vec3 origin = vec3(0, 0.5, -0.4);
    vec3 dir = tPos - origin;
    float l = length(dir);
    float a = (1.0 - (min(l, radius) / radius));
    float f = mix(0.0, strength, a);
    tPos = tPos + (normalize(dir) * f);

	vs_out.normal = normalize(mat3(model_view) * normal);
	vs_out.position = vec3(model_view * vec4(tPos, 1));
	vs_out.texCoord = (texCoord * uv.tiling) + uv.offset;

    gl_Position = projection * view * model * vec4(tPos, 1.0);
}