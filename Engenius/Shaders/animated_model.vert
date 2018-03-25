#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 weights;
layout (location = 7) in mat4 layout_model;

uniform bool instanced;
uniform mat4 uniform_model;
uniform mat4 projection;
uniform mat4 view;

const int MAX_BONES = 50;
uniform mat4 boneLocation[MAX_BONES];
uniform mat4 lightSpaceMatrix;

out VS_OUT {
	vec2 TexCoords;
	vec3 FragPos;
	vec3 Normal;
	vec4 FragPosLightSpace;
} vs_out;

void main()
{	
	mat4 BoneTransform = boneLocation[boneIDs[0]] * weights[0];
	BoneTransform     += boneLocation[boneIDs[1]] * weights[1];
	BoneTransform     += boneLocation[boneIDs[2]] * weights[2];
	BoneTransform     += boneLocation[boneIDs[3]] * weights[3];
	
	mat4 model;
	instanced ? model = layout_model : model = uniform_model;

	vec4 PosL    = BoneTransform * vec4(position, 1.0f);
	gl_Position = projection * view * model * PosL;

	vs_out.FragPos = vec3(model * vec4(position, 1.0f));
	vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    vs_out.TexCoords = texCoords;
	vs_out.Normal = transpose(inverse(mat3(model))) * normal;
	vs_out.Normal = normalize(vs_out.Normal);
} 

