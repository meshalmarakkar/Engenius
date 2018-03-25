#version 330 core

layout(triangles) in;
// Three lines will be generated: 6 vertices
layout(line_strip, max_vertices=6) out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in Vertex
{
  vec3 normal;
} vertex[];

out vec4 vertex_color;

void main()
{
  int i;
  for(i=0; i<gl_in.length(); i++)
  {
    vec3 P = gl_in[i].gl_Position.xyz;
    vec3 N = vertex[i].normal;
    
    gl_Position = projection * view * model * vec4(P, 1.0);
    vertex_color = vec4(1.0, 1.0, 0.0, 1.0);
    EmitVertex();
    
	float normal_length = 2.0f;
    gl_Position = projection * view * model * vec4(P + N * normal_length, 1.0);
    vertex_color = vec4(1.0, 1.0, 0.0, 1.0);
    EmitVertex();
    
    EndPrimitive();
  }
}