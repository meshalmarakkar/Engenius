#version 330 core

const int MAX_PER_LIGHT_TYPE = 3;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

in VS_OUT {
	vec2 TexCoords;
	vec3 FragPos;
	vec3 Normal;
} fs_in;

uniform int pointLightIDs[MAX_PER_LIGHT_TYPE];
uniform int spotLightIDs[MAX_PER_LIGHT_TYPE];

uniform float far_plane;
uniform samplerCube depthMap[MAX_PER_LIGHT_TYPE];
uniform bool depthMap_ifRender[MAX_PER_LIGHT_TYPE];

uniform vec3 viewPos;
uniform bool hasSpecularMap;

layout(location = 0) out vec3 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpecular;
layout(location = 3) out vec3 gPointLightIDs;
layout(location = 4) out vec3 gSpotLightIDs; 

void main()
{    
	vec3 norm = fs_in.Normal;
	vec4 colour4 = texture(texture_diffuse1, fs_in.TexCoords);
	if (colour4.a < 0.5f)
		discard;

	vec3 colour = colour4.rgb;
	vec3 specMap = hasSpecularMap ? texture(texture_specular1, fs_in.TexCoords).rgb : vec3(1.0f);
    
	gPosition = fs_in.FragPos;
	gNormal = norm;
	gAlbedoSpecular.rgb = colour;
	gAlbedoSpecular.a = specMap.r;

	//int casting??
	gPointLightIDs = vec3(pointLightIDs[0], pointLightIDs[1], pointLightIDs[2]);
	gSpotLightIDs = vec3(spotLightIDs[0], spotLightIDs[1], spotLightIDs[2]);
}