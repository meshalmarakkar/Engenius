#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangents;
layout (location = 4) in vec3 bitangents;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 viewPos;
uniform float tiling;
uniform mat4 model;

uniform mat4 lightSpaceMatrix;

out VS_OUT {
	vec2 TexCoords;
	vec3 FragPos;
	mat3 TBN;
	vec3 viewDir;
	vec3 tangentFragPos;
	vec4 FragPosLightSpace;
} vs_out;

void main()
{	
	gl_Position = projection * view * model * vec4(position, 1.0f);
		
	vs_out.FragPos = vec3(model * vec4(position, 1.0f));
	
	vs_out.TexCoords = texCoords * tiling;

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * tangents.xyz);
	vec3 N = normalize(normalMatrix * normal);
	vec3 B = normalize(normalMatrix * bitangents.xyz); 
	//if (dot(cross(N, T), B) < 0.0)
		//T = T * -1.0;
	vs_out.TBN = transpose(mat3(T, B, N));
	
	vec3 tangentViewPos  = vs_out.TBN * viewPos;
	vs_out.tangentFragPos  = vs_out.TBN * vs_out.FragPos;
	vs_out.viewDir = normalize(tangentViewPos - vs_out.tangentFragPos);

	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.tangentFragPos, 1.0);
} 