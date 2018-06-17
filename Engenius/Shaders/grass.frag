#version 330

smooth in vec2 texCoord;
smooth in vec3 worldPos;
smooth in vec4 eyeSpacePos;

out vec4 outColour;

uniform sampler2D texture_diffuse1;
uniform vec4 additionalColour;
uniform float alphaTest;
uniform float alphaMultiplier;

void main(){
	vec4 diffuse = texture2D(texture_diffuse1, texCoord);
	float newAlpha = diffuse.a * alphaMultiplier;
	if (newAlpha < alphaTest)
		discard;
	if (newAlpha > 1.0f)
		newAlpha = 1.0f;

	vec4 finalColour = diffuse * additionalColour;
	outColour = vec4(finalColour.zyx, newAlpha);
}