#version 330 core

layout(location = 0) out vec4 out_Color;

in vec2 texCoords;

uniform sampler2D screenTexture;
uniform vec3 screenSpace_lightPos;

vec4 trialRay(){
	const int NUM_SAMPLES = 100;
	float Exposure = 0.0034f;
	float Decay = 1.0f;
	float Density = 0.84f;
	float Weight = 5.65f;
	
	vec2 alteredTexCoords = texCoords;
	vec2 deltaTexCoord = (texCoords - screenSpace_lightPos.xy);  
	deltaTexCoord *= 1.0f / float(NUM_SAMPLES) * Density;  
	float illuminationDecay = 1.0f; 

	vec4 colour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	for (int i = 0; i < NUM_SAMPLES; i++)  
	{	  
		alteredTexCoords -= deltaTexCoord;  
		vec4 sample = texture(screenTexture, alteredTexCoords);  
		sample *= illuminationDecay * Weight;  
		colour += sample;  
		illuminationDecay *= Decay;  
	}  

	colour *= Exposure;
	return colour;
}

vec4 otherRay(){
	//The width of the blur (the smaller it is the further each pixel is going to sample)
	const float blurWidth = -0.85;
	const int NUM_SAMPLES = 100;

	//compute ray from pixel to light center
    vec2 ray = texCoords - screenSpace_lightPos.xy;
    //output color
    vec4 color = vec4(0.0);

    //sample the texture NUM_SAMPLES times
    for(int i = 0; i < NUM_SAMPLES; i++) {
        //sample the texture on the pixel-to-center ray getting closer to the center every iteration
        float scale = 1.0 + blurWidth * (float(i) / float(NUM_SAMPLES - 1));
        //summing all the samples togheter
        color += texture(screenTexture, (ray * scale) + screenSpace_lightPos.xy) / float(NUM_SAMPLES);
      }

	  return color;
}

void main()
{   
	out_Color = trialRay();
	//out_Color = otherRay();
}
