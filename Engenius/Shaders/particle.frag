// Fragment Shader – file "particle.frag"

#version 330

uniform sampler2D textureMap;
uniform float transparency;

in vec2 texCoord1;
in vec2 texCoord2;
in float blend;

out vec4 outColour;

void main(void) {	
	vec4 texture1 = texture( textureMap, texCoord1 );
	vec4 texture2 = texture( textureMap, texCoord2 );

	//outColour = texture1;
	outColour =  mix(texture1, texture2, blend);
}
