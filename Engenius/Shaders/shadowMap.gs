#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    for(gl_Layer = 0; gl_Layer < 6; ++gl_Layer)
    {
       // gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < gl_in.length(); ++i) // for each triangle's vertices
        {
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadowMatrices[gl_Layer] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
}  