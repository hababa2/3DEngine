#version 430 core

in vec3 fragColor;

out vec4 outColor;

uniform vec3 tint;

void main()
{
    outColor = vec4(fragColor * tint, 1.0);
}