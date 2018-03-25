#version 330

// Some drivers require the following
precision highp float;

in vec2 texCoords;
uniform sampler2D textureMap;
uniform bool allowLowTransparency;
uniform float transparency;

// Ouput data
out vec4 out_colour;

void main(void) {
    
	// Fragment colour
	vec4 colours = texture( textureMap, texCoords );

	if (transparency > 0.0f)
		colours.a = transparency;
	else if (colours.a < 0.5f && allowLowTransparency == false){
		discard;	// discard texels with alphaof less than a certain float
	}
			
	out_colour =  colours;
		
}