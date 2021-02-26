#version 120
in vec3 vertex_pos;
in vec3 in_color;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec3 color;

void main()
{
	gl_Position =  projection * view * model * vec4(vertex_pos,1.0);
	color = in_color;
}