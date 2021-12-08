#version 430 core 

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

flat out vec3 fragColor;
out vec2 fragTexCoord;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 position;
};

uniform Material material;
uniform Light light;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat4 model_view = view * model;

	vec3 ambient = material.ambient * light.ambient;

	vec3 vNorm = mat3(model_view) * normal;
	vec4 vPos = model_view * vec4(position, 1);
	vec3 lDir = normalize(vec3(light.position - vPos));

	float intensity = max(dot(lDir, vNorm), 0);
	vec3 diffuse = material.diffuse * light.diffuse * intensity;

	vec3 specular = vec3(0);
	if(intensity > 0)
	{
		vec3 viewDir = normalize(-vec3(vPos));
		
		vec3 reflection = reflect(-lDir, vNorm);
        intensity = max(dot(viewDir, reflection), 0);
        intensity = pow(intensity, material.shininess);
        specular = material.specular * light.specular * intensity;
	}

	fragColor = ambient + diffuse + specular;
	fragTexCoord = texCoord;
    gl_Position = projection * view * model * vec4(position, 1.0);
}