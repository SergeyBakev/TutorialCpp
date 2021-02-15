#version 120
vec3 v_uv;
 
void main()
{
  gl_Position = gl_Vertex;
  v_uv = gl_MultiTexCoord0;
}