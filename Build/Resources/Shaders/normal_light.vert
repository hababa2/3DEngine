#version 430 core 

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 tangent;

out VertOut
{
	vec3 position;
	vec2 texCoord;
	vec3 lPos;
} vertOut;

uniform vec4 lightPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat4 model_view = view * model;
	mat3 normal_matrix = transpose(inverse(mat3(model_view)));

	vec3 N = normalize(normal_matrix * normal);
	vec3 T = normalize(normal_matrix * tangent);
	T = normalize(T - dot(T, N) * N);
	vec3 B = normalize(cross(N, T));
	
	mat3 tbn = transpose(mat3(T, B, N));
	vertOut.position = tbn * vec3(model_view * vec4(position, 1.0));
	vertOut.lPos = tbn * vec3(lightPosition);
	vertOut.texCoord = texCoord;

	vertOut.position = vec3(model_view * vec4(position, 1));
	vertOut.texCoord = texCoord;

    gl_Position = projection * view * model * vec4(position, 1.0);
}