#version 120
in vec3 vertex_pos;
in vec3 vertex_color;
out vec3 color;

void main()
{
	color = vertex_color;
	gl_Position =  vec4(vertex_pos,1.0);
}