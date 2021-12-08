#version 430 core

layout(location = 0) flat in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

out vec4 outColor;

uniform sampler2D texSampler;

void main()
{
    outColor = texture(texSampler, fragTexCoord) * vec4(fragColor, 1);
}