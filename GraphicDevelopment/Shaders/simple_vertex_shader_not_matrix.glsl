#version 120
in vec3 vertex_pos;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main()
{
	gl_Position =  vec4(vertex_pos,1.0);
}