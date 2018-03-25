#version 330 core

layout(location = 0) out vec4 out_Color;

in vec2 texCoords;

uniform sampler2D screenTexture;
uniform sampler2D bloomBlur;
uniform vec3 screenSpace_lightPos;

const float offset = 1.0 / 300.0; 

uniform bool bloom;
uniform float exposure; 
uniform int effect;

vec3 effects(vec3 colour);
vec3 highDynamicRange(vec3 colour);

vec3 godRay(vec3 colour){
	const float blurWidth = -0.85;
	const int NUM_SAMPLES = 100;

    //compute ray from pixel to light center
    vec2 ray = texCoords - screenSpace_lightPos.xy;
    //output color
    vec3 color = vec3(0.0);

    //sample the texture NUM_SAMPLES times
    for(int i = 0; i < NUM_SAMPLES; i++) {
        //sample the texture on the pixel-to-center ray getting closer to the center every iteration
        float scale = 1.0 + blurWidth * (float(i) / float(NUM_SAMPLES - 1));
        //summing all the samples togheter
        color += (texture2D(screenTexture, (ray * scale) + screenSpace_lightPos.xy).xyz) / float(NUM_SAMPLES);
    }

	return color;
}

void main()
{    
	vec3 colour = texture(screenTexture, texCoords).rgb;
	colour = highDynamicRange(colour);
	if(effect > 0)
		if(effect < 5)
			colour += effects(colour);

	float gamma = 2.2;
	colour = pow(colour, vec3(1.0 / gamma)); //gamma correction

	//colour = godRay(colour);
	out_Color = vec4(colour, 1.0);
}

vec3 highDynamicRange(vec3 colour){
	
	vec3 result = vec3(0.0);
	vec3 hdrColour = colour.rgb;
	if(bloom){
		vec3 bloomColour = texture(bloomBlur, texCoords).rgb;
        hdrColour += bloomColour * 0.5f; // additive blending. TAKE AWAY 0.5f for full effect
	}
	//before tone mapping so blur can hdr to ldr good as well
    
	//tone mapping
    result = vec3(1.0) - exp(-hdrColour * exposure);

	return result;
}

vec3 effects(vec3 colour){
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

	float blur[9] = float[](
		//BLUR EFFECT
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	);

	float retro[9] = float[](
		//RETRO?? SHARPEN??
		-1, -1, -1,
		-1,  9, -1,
		-1, -1, -1
	);

	float intense[9] = float[](
		//more intense version
		2, 2, 2,
		2, -15, 2,
		2, 2, 2
	);

	float edge[9] = float[](
		//EDGE DETECTION??
		1, 1, 1,
		1, -9, 1,
		1, 1, 1
	);
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
    }

	vec3 col = vec3(0.0);
	for(int i = 0; i < 9; i++){
		if (effect == 1)
			col += sampleTex[i] * blur[i];
		if (effect == 2)
			col += sampleTex[i] * retro[i];
		if (effect == 3)
			col += sampleTex[i] * intense[i];
		if (effect == 4)
			col += sampleTex[i] * edge[i];
	}

	//GRAYSCALE

	//float average = (colour.r + colour.g + colour.b) / 3.0;
	//Human eye tends to be more sensitive to green colors and the least to blue, so to get the most physically accurate results we'll need to use weighted channels:
	float average = 0.2126 * colour.r + 0.7152 * colour.g + 0.0722 * colour.b;
	//col = vec3(average, average, average);

	//INVERSED COLOURS
	//col = vec3(1.0 - colour);

	return col;
}
