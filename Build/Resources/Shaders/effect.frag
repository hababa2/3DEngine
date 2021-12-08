#version 430 core

in VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texCoord;
} vs_out;

out vec4 outColor;

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

uniform sampler2D texSampler;
uniform float time;

float random(float n)
{
    return fract(sin(n) * 43758.5453123);
}

void main()
{
	vec3 ambient = material.ambient * light.ambient;
	
	vec3 lDir = normalize(vec3(light.position) - vs_out.position);

	float intensity = max(dot(lDir, vs_out.normal), 0);
	vec3 diffuse = material.diffuse * light.diffuse * intensity;

	vec3 specular = vec3(0);
	if(intensity > 0)
	{
		vec3 viewDir = normalize(-vec3(vs_out.position));
		
		vec3 reflection = reflect(-lDir, vs_out.normal);
        intensity = max(dot(viewDir, reflection), 0);
        intensity = pow(intensity, material.shininess);
        specular = material.specular * light.specular * intensity;
	}

	vec4 color = vec4(ambient + diffuse, 1) * texture(texSampler, vs_out.texCoord) + vec4(specular, 1);
	//color = color.rrrr;
	color = vec4(1) - color;
	color *= random(gl_FragCoord.x * gl_FragCoord.y * time);

	outColor = color;
}