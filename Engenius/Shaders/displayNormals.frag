#version 330 core

in vec4 vertex_color;
out vec4 Out_Color;
void main()
{
  Out_Color = vertex_color;
}