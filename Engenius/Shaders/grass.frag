#version 330

smooth in vec2 texCoord;

out vec4 outColour;

uniform sampler2D texture_diffuse1;
uniform float alphaTest;

void main(){
	vec4 diffuse = texture2D(texture_diffuse1, texCoord);
	float newAlpha = diffuse.a;
	if (newAlpha < alphaTest)
		discard;
	if (newAlpha > 1.0f)
		newAlpha = 1.0f;

	vec4 finalColour = diffuse;
	outColour = vec4(finalColour.xyz, newAlpha);
}