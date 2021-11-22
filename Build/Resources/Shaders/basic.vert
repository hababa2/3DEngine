#version 430 core 

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

out vec3 fragColor;
out vec2 fragTexCoord;

uniform float scale;

void main()
{
	fragColor = color;
	fragTexCoord = texCoord;
    gl_Position = vec4(position * scale, 1.0);
}