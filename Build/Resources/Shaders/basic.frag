#version 430 core

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

out vec4 outColor;

uniform vec3 tint;
uniform sampler2D texSampler;

void main()
{
    outColor = vec4(fragColor * tint, 1.0) * texture(texSampler, fragTexCoord);
}