#version 120
in vec3 color;
uniform float ambientStrength;
uniform vec3 lightColor;
void main()
{
	vec3 ambient = ambientStrength * lightColor;
	vec3 result = ambient * color;
	gl_FragColor = vec4(result,1.0);
}