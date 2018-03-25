#version 330 core
layout(location = 0) out vec4 out_Color;
layout(location = 1) out vec4 toBlur_out_Color;
uniform vec4 colour;
void main()
{
	out_Color = colour;
	toBlur_out_Color = vec4(0.0, 0.0, 0.0, 1.0);
}	