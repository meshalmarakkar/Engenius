#version 330 core

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
const int MAX_PER_LIGHT_TYPE = 3;

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpecular;
uniform sampler2D gPointLightIDs;
uniform sampler2D gSpotLightIDs;

in vec2 texCoords;

uniform PointLight pointLights[5];
uniform SpotLight spotLights[MAX_PER_LIGHT_TYPE];

uniform float far_plane;
uniform samplerCube depthMap[MAX_PER_LIGHT_TYPE];
uniform bool depthMap_ifRender[MAX_PER_LIGHT_TYPE];

uniform vec3 viewPos;
uniform bool displayShadow;

layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec4 toBlur_out_Color;

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   

////-- Function prototypes --////
float ShadowCalculation(vec3 lightPosition, vec3 fragPos, samplerCube shadowCube);
vec3 CalcPointLight(PointLight light, vec3 viewDir, vec3 fragPos, vec3 norm, float shadow, vec3 colour, vec3 specMap);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 colour, vec3 specMap);

float near = 0.01; 
float far  = 200.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{    
    // Properties 
	vec3 fragPos = texture(gPosition, texCoords).rgb;
	vec3 norm = texture(gNormal, texCoords).rgb;
	vec3 colour = texture(gAlbedoSpecular, texCoords).rgb;
	vec3 specMap = vec3(texture(gAlbedoSpecular, texCoords).a);
	vec3 ptLightIDs = texture(gPointLightIDs, texCoords).rgb;
	vec3 spLightIDs = texture(gSpotLightIDs, texCoords).rgb;

	vec3 viewDir = normalize(viewPos - fragPos);
	int pointLightIDs[MAX_PER_LIGHT_TYPE] = {int(ptLightIDs.x), int(ptLightIDs.y), int(ptLightIDs.z)};
	int spotLightIDs[MAX_PER_LIGHT_TYPE] = {int(spLightIDs.x), int(spLightIDs.y), int(spLightIDs.z)};

    float shadow = 0.0;
	vec3 result;

	for(int i = 0; i < MAX_PER_LIGHT_TYPE; i++){
		shadow = 0.0f;//displayShadow && depthMap_ifRender[i] ? ShadowCalculation(pointLights[i].position, fragPos, depthMap[i]) : 0.0;	
		result += pointLightIDs[i] > -1 ? CalcPointLight(pointLights[pointLightIDs[i]], viewDir, fragPos, norm, shadow, colour, specMap) : vec3(0.0f);   
		result += spotLightIDs[i] > -1 ? CalcSpotLight(spotLights[spotLightIDs[i]], norm, fragPos, viewDir, colour, specMap) : vec3(0.0f);
	}
	
	//eyes prefer green most and blue least
	float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
		toBlur_out_Color = vec4(result, 1.0);
	else
		toBlur_out_Color = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	out_Color = vec4(result, 1.0);
	//out_Color = vec4(colour, 1.0);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 viewDir, vec3 fragPos, vec3 norm, float shadow, vec3 colour, vec3 specMap)
{  
	vec3 lightDir = normalize(light.position - fragPos);
	
	// Ambient shading
	vec3 ambient = light.ambient * colour;

	// Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * colour;

	// Specular shading
    //Blinn-Phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0f);
	//Phong
	//vec3 reflectDir = reflect(-lightDir, norm);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0f);
    vec3 specular = light.specular * spec * specMap;

	// Attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	// Combine results
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

     return (ambient + (1.0 - shadow) * (diffuse + specular));
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 colour, vec3 specMap)
{
    vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction)); 
    
	if(theta > light.outerCutOff){ //or just cutOff
		// diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);

		// Specular shading
		//Blinn-Phong
		vec3 halfwayDir = normalize(lightDir + viewDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
		//Phong
		//vec3 reflectDir = reflect(-lightDir, normal);
		//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0f);

		// attenuation
		float distance = length(light.position - fragPos);
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

		// spotlight intensity
		float epsilon = light.cutOff - light.outerCutOff;
		float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

		// combine results
		vec3 ambient = light.ambient * colour;
		vec3 diffuse = light.diffuse * diff * colour;
		vec3 specular = light.specular * spec * specMap;

		ambient *= attenuation * intensity;
		diffuse *= attenuation * intensity;
		specular *= attenuation * intensity;

		return (ambient + diffuse + specular);
	}
	
	return vec3(0.0f, 0.0f, 0.0f);
}


// Calculate shadows
float ShadowCalculation(vec3 lightPosition, vec3 fragPos, samplerCube shadowCube)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPosition;

    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);

    // Now test for shadows
    float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(viewPos - fragPos);
	float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;  
	for(int i = 0; i < samples; ++i)
	{
		float closestDepth = texture(shadowCube, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= far_plane;   // Undo mapping [0;1]
		if(currentDepth - bias > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(samples);  

    return shadow;
}
