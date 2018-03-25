#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangents;
layout (location = 4) in vec3 bitangents;
layout (location = 7) in mat4 layout_model;
layout (location = 11) in vec2 pointIDs;
layout (location = 12) in vec2 spotIDs;

uniform bool instanced;
uniform mat4 uniform_model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 viewPos;
uniform float tiling;

out VS_OUT {
	vec2 TexCoords;
	vec3 FragPos;
	mat3 TBN;
	vec3 viewDir;
	vec3 tangentFragPos;
} vs_out;

void main()
{
	mat4 model;
	instanced ? model = layout_model : model = uniform_model;
	
	gl_Position = projection * view * model * vec4(position, 1.0f);
		
	vs_out.FragPos = vec3(model * vec4(position, 1.0f));
	
	vs_out.TexCoords = texCoords * tiling;

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * tangents.xyz);
	vec3 N = normalize(normalMatrix * normal);
	vec3 B = normalize(normalMatrix * bitangents.xyz); 
	//if (dot(cross(N, T), B) < 0.0)
		//T = T * -1.0;
	//vec3 B = cross(N, T);
	vs_out.TBN = transpose(mat3(T, B, N));
	
	vec3 tangentViewPos  = vs_out.TBN * viewPos;
	vs_out.tangentFragPos  = vs_out.TBN * vs_out.FragPos;
	vs_out.viewDir = normalize(tangentViewPos - vs_out.tangentFragPos);
} 