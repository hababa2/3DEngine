#version 430 core 

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec3 fragColor;
out vec2 fragTexCoord;

struct Color
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Color material;
uniform Color light;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec3 ambient = material.ambient * light.ambient;

	fragColor = ambient;
	fragTexCoord = texCoord;
    gl_Position = projection * view * model * vec4(position, 1.0);
}