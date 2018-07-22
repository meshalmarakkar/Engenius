#version 330 core

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform bool hasSpecularMap;
const int MAX_PER_LIGHT_TYPE = 3;
uniform int pointLightIDs[MAX_PER_LIGHT_TYPE];
uniform int spotLightIDs[MAX_PER_LIGHT_TYPE];

uniform bool instanced;

in VS_OUT {
	vec2 TexCoords;
	vec3 FragPos;
	mat3 TBN;
	ivec3 pIDs;
	ivec3 sIDs;
} fs_in;


layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpecular;
layout(location = 3) out ivec3 gPointLightIDs;
layout(location = 4) out ivec3 gSpotLightIDs;

layout(location = 5) out vec3 gTBN_T;
layout(location = 6) out vec3 gTBN_B;
layout(location = 7) out vec3 gTBN_N;

void main()
{    	
	vec3 norm = normalize( (texture(texture_normal1, fs_in.TexCoords).rgb) * 2.0 - 1.0);
	vec3 colour = texture(texture_diffuse1, fs_in.TexCoords).rgb;
	vec3 specMap = hasSpecularMap ? texture(texture_specular1, fs_in.TexCoords).rgb : vec3(1.0f);

	gPosition = fs_in.FragPos;
	gNormal = norm;
	// diffuse per-fragment color
	gAlbedoSpecular.rgb = colour;
	// store specular intensity in alpha component
	gAlbedoSpecular.a = specMap.r;

	//FOR MAPPING
	gTBN_T = vec3(fs_in.TBN[0][0], fs_in.TBN[0][1], fs_in.TBN[0][2]);
	gTBN_B = vec3(fs_in.TBN[1][0], fs_in.TBN[1][1], fs_in.TBN[1][2]);
	gTBN_N = vec3(fs_in.TBN[2][0], fs_in.TBN[2][1], fs_in.TBN[2][2]);

	//gPointLightIDs = instanced ? fs_in.pIDs : ivec3(pointLightIDs[0], pointLightIDs[1], pointLightIDs[2]);
	//gSpotLightIDs = instanced ? fs_in.sIDs : ivec3(spotLightIDs[0], spotLightIDs[1], spotLightIDs[2]);
		
	gPointLightIDs = instanced ? ivec3(0,1,2) : ivec3(pointLightIDs[0], pointLightIDs[1], pointLightIDs[2]);
	gSpotLightIDs = instanced ? ivec3(-1,-1,-1) : ivec3(spotLightIDs[0], spotLightIDs[1], spotLightIDs[2]);
}