#version 330 core

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

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

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

in VS_OUT {
	vec2 TexCoords;
	vec3 FragPos;
	mat3 TBN;
	vec3 viewDir;
	vec3 tangentFragPos;
	vec4 FragPosLightSpace;
} fs_in;

uniform DirLight dirLight;
uniform PointLight pointLights[MAX_PER_LIGHT_TYPE];
uniform SpotLight spotLights[MAX_PER_LIGHT_TYPE];

uniform int pointLightIDs[MAX_PER_LIGHT_TYPE];
uniform int spotLightIDs[MAX_PER_LIGHT_TYPE];

uniform float far_plane;
uniform samplerCube depthMap[MAX_PER_LIGHT_TYPE];
uniform bool depthMap_ifRender[MAX_PER_LIGHT_TYPE];
uniform sampler2D depthMap_DirLight;

uniform vec3 viewPos;
uniform bool hasSpecularMap;
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
float ShadowCalculation(vec3 lightPosition, samplerCube shadowCube);
float ShadowCalculation_DirLight(vec4 fragPosLightSpace, vec3 normal);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 colour, vec3 specMap, float shadow);
vec3 CalcPointLight(PointLight light, vec3 viewDir, vec3 norm, float shadow, vec3 colour, vec3 specMap);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 colour, vec3 specMap);

void main()
{    
    // Properties
	vec2 newTexCoords = fs_in.TexCoords; 
	vec3 norm = normalize( (texture(texture_normal1, newTexCoords).rgb) * 2.0 - 1.0);
	vec3 colour = texture(texture_diffuse1, newTexCoords).rgb;
	vec3 specMap = hasSpecularMap ? texture(texture_specular1, fs_in.TexCoords).rgb : vec3(1.0f);

    float shadow = 0.0;
//	shadow = displayShadow ? ShadowCalculation_DirLight(fs_in.FragPosLightSpace, norm) : 0.0;
	vec3 result ;//= CalcDirLight(dirLight, norm, fs_in.viewDir, colour, specMap, shadow);

	for(int i = 0; i < MAX_PER_LIGHT_TYPE; i++){
		shadow = displayShadow && depthMap_ifRender[i] ? ShadowCalculation(pointLights[i].position, depthMap[i]) : 0.0;	
		result += pointLightIDs[i] > -1 ? CalcPointLight(pointLights[pointLightIDs[i]], fs_in.viewDir, norm, shadow, colour, specMap) : vec3(0.0f);   
		result += spotLightIDs[i] > -1 ? CalcSpotLight(spotLights[spotLightIDs[i]], norm, fs_in.FragPos, fs_in.viewDir, colour, specMap) : vec3(0.0f);
	}
	
	//eyes prefer green most and blue least
	float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
//    if(brightness > 1.0)
 //       toBlur_out_Color = vec4(result, 1.0);
//	else
		toBlur_out_Color = vec4(0.0, 0.0, 0.0, 1.0);

	out_Color = vec4(result, 1.0);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 viewDir, vec3 norm, float shadow, vec3 colour, vec3 specMap)
{  
	vec3 tangentLightPos = fs_in.TBN * light.position;
	vec3 lightDir = normalize(tangentLightPos - fs_in.tangentFragPos);

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
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));  
    
	// Combine results
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + (1.0 - shadow) * (diffuse + specular));
}


// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 colour, vec3 specMap, float shadow)
{
	//vec3 tangentLightPos = fs_in.TBN * -vec3(light.direction);
	//vec3 lightDir = normalize(tangentLightPos - fs_in.tangentFragPos);
	vec3 tangentLightPos = fs_in.TBN * vec3(light.direction);
	vec3 lightDir = -normalize(tangentLightPos - fs_in.tangentFragPos);
    //vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    //Blinn-Phong
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
	//Phong
	//vec3 reflectDir = reflect(-lightDir, normal);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0f);
    // combine results
    vec3 ambient = light.ambient * colour;
    vec3 diffuse = light.diffuse * diff * colour;
    vec3 specular = light.specular * spec * specMap;
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 colour, vec3 specMap)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction)); 
    
	//vec3 tangentLightPos = fs_in.TBN * light.position;
	//lightDir = normalize(tangentLightPos - fs_in.tangentFragPos);
	
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
float ShadowCalculation(vec3 lightPosition, samplerCube shadowCube)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fs_in.FragPos - lightPosition;

    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
	
    // Now test for shadows
    float shadow = 0.0;
	float bias = 0.15;
	int samples = 20;
	float viewDistance = length(viewPos - fs_in.FragPos);
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

float ShadowCalculation_DirLight(vec4 fragPosLightSpace, vec3 normal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(depthMap_DirLight, projCoords.xy).r; 

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // calculate bias (based on depth map resolution and slope)
    vec3 lightDir = normalize(dirLight.direction - fs_in.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(depthMap_DirLight, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(depthMap_DirLight, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

