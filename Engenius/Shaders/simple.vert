#version 330 core
layout (location = 0) in vec3 position;
layout (location = 7) in mat4 layout_model;

uniform mat4 uniform_model;
uniform mat4 view;
uniform mat4 projection;
uniform bool instanced;

void main()
{   
	mat4 model;
	instanced ? model = layout_model : model = uniform_model;

    gl_Position = projection * view * model * vec4(position, 1.0f);
}