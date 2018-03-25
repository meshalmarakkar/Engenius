#version 330 core

layout (location = 0) in vec3 position;
layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 weights;
layout (location = 7) in mat4 layout_model;

uniform bool instanced;
uniform mat4 uniform_model;

const int MAX_BONES = 50;
uniform mat4 boneLocation[MAX_BONES];

uniform bool animated;

void main()
{
	mat4 model;
	instanced ? model = layout_model : model = uniform_model;

	if (animated == true){
		mat4 BoneTransform = boneLocation[boneIDs[0]] * weights[0];
		BoneTransform     += boneLocation[boneIDs[1]] * weights[1];
		BoneTransform     += boneLocation[boneIDs[2]] * weights[2];
		BoneTransform     += boneLocation[boneIDs[3]] * weights[3];
		vec4 PosL    = BoneTransform * vec4(position, 1.0f);
		gl_Position = model * PosL;
		}
	else
		gl_Position = model * vec4(position, 1.0);
}  