#version 120
varying vec4 v_uv;
uniform vec4 params;
 
// adapted from https://thebookofshaders.com/edit.php#10/ikeda-simple-grid.frag
 
float grid(vec2 st, float res)
{
  vec2 grid = fract(st*res);
  return (step(res,grid.x) * step(res,grid.y));
}
 
void main()
{
  vec2 grid_uv = v_uv.xy * params.x; // scale
  float x = grid(grid_uv, params.y); // resolution
  gl_FragColor.rgb = vec3(0.5) * x;  
  gl_FragColor.a = 1.0;
}