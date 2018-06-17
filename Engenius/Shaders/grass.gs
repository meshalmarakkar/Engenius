#version 330

layout (points) in;
layout (triangle_strip, max_vertices = 12) out;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform float timePassed;

smooth out vec2 texCoord;
smooth out vec3 worldPos;
smooth out vec4 eyeSpacePos;

vec3 localSeed;

mat4 rotationMatrix(vec3 axis, float angle);
float randZeroOne();
int randomInt(int min, int max);

void main()
{
	mat4 matrix_MV = view * model;
	mat4 matrix_MVP = projection * matrix_MV;

	vec3 grassFieldPos = gl_in[0].gl_Position.xyz;

	float PIover180 = 3.1415/180.0;
	vec3 baseDirection[] =
	{
		vec3(1.0, 0.0, 0.0),
		vec3(float(cos(45.0 * PIover180)), 0.0f, float(sin(45.0 * PIover180))),
		vec3(float(cos(-45.0 * PIover180)), 0.0f, float(sin(-45.0 * PIover180)))
	};

	float grassPatchSize = 5.0f;
	float windStrength = 4.0f;
	
	vec3 windDirection = vec3(1.0, 0.0, 1.0);
	windDirection = normalize(windDirection);

	for (int i = 0; i < 3; i++){
		// grass patch top left vertex
		vec3 baseDirectionRotated = (rotationMatrix(vec3(0, 1, 0), sin(timePassed * 0.7f) * 0.1f) * vec4(baseDirection[i], 1.0)).xyz;
		
		localSeed = grassFieldPos * float(i);
		int grassPatch = randomInt(0, 3);
		
		float grassPatchHeight = 3.5f + randZeroOne() * 2.0f;
	
		float topCentreStartX = float(grassPatch) * 0.25f;
		float topCentreEndX = topCentreStartX+0.25f;
		
		float windPower = 0.5f + sin(grassFieldPos.x / 30 + grassFieldPos.z / 30 + timePassed * (1.2f + windStrength / 20.0f));
		if(windPower < 0.0f)
			windPower = windPower * 0.2f;
		else 
			windPower = windPower * 0.3f;
		
		windPower *= windStrength;

		vec3 topLeft = grassFieldPos - baseDirectionRotated * grassPatchSize * 0.5f + windDirection * windPower;
		topLeft.y += grassPatchHeight;   
		gl_Position = matrix_MVP * vec4(topLeft, 1.0);
		texCoord = vec2(topCentreStartX, 1.0);
		worldPos = topLeft;
		eyeSpacePos = matrix_MV * vec4(topLeft, 1.0);
		EmitVertex();
		
		// Grass patch bottom left vertex
		vec3 bottomLeft = grassFieldPos - baseDirection[i] * grassPatchSize*0.5f;  
		gl_Position = matrix_MVP * vec4(bottomLeft, 1.0);
		texCoord = vec2(topCentreStartX, 0.0);
		worldPos = bottomLeft;
		eyeSpacePos = matrix_MV * vec4(bottomLeft, 1.0);
		EmitVertex();
		                               
		// Grass patch top right vertex
		vec3 topRIght = grassFieldPos + baseDirectionRotated * grassPatchSize * 0.5f + windDirection * windPower;
		topRIght.y += grassPatchHeight;  
		gl_Position = matrix_MVP * vec4(topRIght, 1.0);
		texCoord = vec2(topCentreEndX, 1.0);
		worldPos = topRIght;
		eyeSpacePos = matrix_MV * vec4(topRIght, 1.0);
		EmitVertex();
		
		// Grass patch bottom right vertex
		vec3 bottomRight = grassFieldPos + baseDirection[i] * grassPatchSize * 0.5f;  
		gl_Position = matrix_MVP * vec4(bottomRight, 1.0);
		texCoord = vec2(topCentreEndX, 0.0);
		worldPos = bottomRight;
		eyeSpacePos = matrix_MV * vec4(bottomRight, 1.0);
		EmitVertex();
		
		EndPrimitive();
	}
}

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}



// This function returns random number from zero to one
float randZeroOne()
{
    uint n = floatBitsToUint(localSeed.y * 214013.0 + localSeed.x * 2531011.0 + localSeed.z * 141251.0);
    n = n * (n * n * 15731u + 789221u);
    n = (n >> 9u) | 0x3F800000u;
 
    float fRes =  2.0 - uintBitsToFloat(n);
    localSeed = vec3(localSeed.x + 147158.0 * fRes, localSeed.y * fRes  + 415161.0 * fRes, localSeed.z + 324154.0 * fRes);
    return fRes;
}

int randomInt(int min, int max)
{
	float randomFloat = randZeroOne();
	return int(float(min) + randomFloat * float(max - min));
}