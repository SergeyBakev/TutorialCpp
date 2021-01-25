#version 120
in vec3 vertex_pos;
in vec3 vertex_color;
out vec3 color;
uniform mat4 projection;
uniform mat4 model;

void main()
{
	color = vertex_color;
	gl_Position =  projection * model * vec4(vertex_pos,1.0);
}