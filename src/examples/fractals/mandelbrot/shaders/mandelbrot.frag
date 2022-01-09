#version 410 core


uniform float time;
uniform vec2 resolution;
out vec4 color;
void main()
{
  float zoom = pow(10, -time/100);
  
  vec2 uv = (gl_FragCoord.xy-0.5*resolution.xy)/resolution.y;

  vec2 c = uv*zoom*3;
  c += vec2(-0.69955, 0.38);
  vec2 z = vec2(0);
  float iter = 0;

  const float max_iter = 100;
  for (float i = 0;i <max_iter;i++)
  {
    z = vec2(z.x*z.x - z.y * z.y, 2*z.x*z.y) + c;
    if(length(z) > 2) break;
    iter ++;
  }
  vec3 col = vec3(iter/max_iter);
    color = vec4(col, 1);
}