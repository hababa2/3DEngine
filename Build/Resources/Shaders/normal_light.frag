#version 430 core

in VertOut
{
	vec3 position;
	vec2 texCoord;
	vec3 lPos;
} fragIn;

out vec4 outColor;

struct Material
{
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

layout (binding = 0) uniform sampler2D colorSample;
layout (binding = 1) uniform sampler2D normSample;

void main()
{
	// generate the normals from the normal map
	vec3 normal = texture(normSample, fragIn.texCoord).rgb;
	// convert rgb (0 <-> 1) to xyx (-1 <-> 1)
	normal = normalize(normal * 2.0 - 1.0);

	//ambient
	vec3 ambient = light.ambient;
	
	// diffuse
	vec3 lDir = normalize(vec3(fragIn.lPos) - fragIn.position);
	float intensity = max(dot(lDir, normal), 0);
	vec3 diffuse = material.diffuse * light.diffuse * intensity;

	vec3 specular = vec3(0);
	if(intensity > 0)
	{
		vec3 viewDir = normalize(-vec3(fragIn.position));
		
		vec3 reflection = reflect(-lDir, normal);
        intensity = max(dot(viewDir, reflection), 0);
        intensity = pow(intensity, material.shininess);
        specular = material.specular * light.specular * intensity;
	}

	outColor = vec4(ambient + diffuse, 1) * texture(colorSample, fragIn.texCoord) + vec4(specular, 1);
}