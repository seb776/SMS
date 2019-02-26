#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform float time;
uniform int pointerCount;
uniform vec3 pointers[10];
uniform vec2 resolution;
uniform vec2 cameraAddent;
uniform mat2 cameraOrientation;
uniform samplerExternalOES cameraFront;

vec3 render(vec2 uv, float mx)
{
  vec3 color = vec3(
    uv,
    0.25 + 0.5 * sin(time) );

  for (int n = 0; n < pointerCount; ++n) {
    vec3 hole = vec3(sin(1.5 - distance(uv,pointers[n].xy / mx) * 8.0));

    color = mix(color, hole, 0.5);
  }
  return color;
}
vec2 myPixel(vec2 uv, float coef)
{
  float coefL = coef;//0.05;//mix(0.2, 0.012, cos(time/1.14)*0.5+0.5);
  float cx = coefL;
  float cy = cx;
  vec2 coef= vec2(cx, cy);
  vec2 coefed = uv *coef;
  return (coefed - fract(coefed))/coef;
}
void main(void) {
  float mx = max(resolution.x, resolution.y);

  vec2 uv = gl_FragCoord.xy / mx;

  uv.x /= (resolution.x/resolution.y);
uv.x= 1.0-uv.x;
  uv= myPixel(uv,40.0);
vec2 st = cameraAddent + uv * cameraOrientation;

  vec3 color = render(uv, mx);
  color *= texture2D(cameraFront, st).xyz;
  gl_FragColor = vec4(color, 1.0);
}
