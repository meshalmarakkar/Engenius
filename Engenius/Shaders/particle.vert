#version 330

layout (location = 0) in vec2 vertices;
layout (location = 1) in vec3 position;
layout (location = 2) in vec4 texOffsets;
layout (location = 3) in float blendFactor;

out vec2 texCoord1;
out vec2 texCoord2;
out float blend;

uniform float size;
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP; // Model-View-Projection matrix, but without the Model (the position is in BillboardPos; the orientation depends on the camera)
uniform unsigned int number_of_rows;

void main(void)
{
	vec2 texCoord = vertices + vec2(0.5, 0.5);
	texCoord.y = 1.0 - texCoord.y;
	texCoord /= number_of_rows;
	texCoord1 = texCoord + texOffsets.xy;
	texCoord2 = texCoord + texOffsets.zw;
	blend = blendFactor;

	vec3 vertexPosition_worldspace = 
		position
		+ CameraRight_worldspace * vertices.x * size
		+ CameraUp_worldspace * vertices.y * size;
	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0);
}