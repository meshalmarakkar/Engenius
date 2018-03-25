#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 7) in mat4 layout_model;

uniform bool instanced;
uniform mat4 uniform_model;
uniform mat4 projection;
uniform mat4 view;
uniform float tiling;

uniform mat4 lightSpaceMatrix;

out VS_OUT {
	vec2 TexCoords;
	vec3 FragPos;
	vec3 Normal;
	vec4 FragPosLightSpace;
} vs_out;

void main()
{	
	mat4 model;
	instanced ? model = layout_model : model = uniform_model;
		
	gl_Position = projection * view * model * vec4(position, 1.0f);

	vs_out.FragPos = vec3(model * vec4(position, 1.0f));
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    vs_out.TexCoords = texCoords * tiling;
	vs_out.Normal = transpose(inverse(mat3(model))) * normal;
	vs_out.Normal = normalize(vs_out.Normal);
} 


