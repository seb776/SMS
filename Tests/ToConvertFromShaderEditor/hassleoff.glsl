#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

#define PI 3.1415
#define PI2 (2.0*3.1415)

uniform vec2 resolution;
uniform sampler2D Hassle;

uniform float time;

float drawN(vec2 pos, vec2 uv)
{
  return 0.0;
}

vec4 drawHassle(vec2 uv, vec2 pos, vec2 size, float angle)
{
  float sinAngle = sin(angle);
  float cosAngle = cos(angle);
  vec4 uvLim = vec4(vec2(-2.0), vec2(2.0));
  mat2 rotation = mat2(cosAngle, -sinAngle, sinAngle, cosAngle);
uvLim.xy += vec2(-pos.x, pos.y);
uvLim.zw += vec2(-pos.x, pos.y);
  uvLim = vec4(rotation *uvLim.xy, rotation * uvLim.zw);
  if (uv.x >uvLim.x && uv.x <uvLim.z &&
    uv.y > uvLim.y && uv.y < uvLim.w)
  {
    uv += vec2(-pos.x, pos.y)+vec2(0.5,0.0);
    vec3 hassleCol = texture2D(Hassle, rotation * uv).rgb;
    if (hassleCol.g <0.999)
      return vec4(hassleCol,1.0);
  }
  return vec4(0.0);
}

vec3 flare(vec2 uv)
{
  const float maxLength = 1.0;
  vec3 colF = vec3(0.123, 0.234, 0.789);

  return mix(vec3(0.0), colF, 1.0 - (length(uv) / maxLength));
}

vec3 render(vec2 uv)
{
  return vec3(0.0);
}

void main(void) {
  vec2 uv = gl_FragCoord.xy / resolution.xy;
  vec2 uvRatio = vec2(uv.x, uv.y / (resolution.x/resolution.y));
  float ratio = resolution.x/resolution.y;
  uv.x *= ratio;

  vec2 center = vec2(0.5,1.0/(resolution.x/resolution.y)/2.0);
  vec2 diff = uvRatio -center;
  vec2 centerUvRatio = vec2(uvRatio.x, uvRatio.y-center.y-0.5);

  vec2 normDiff = normalize(diff);
  float angle = atan(normDiff.x ,normDiff.y)-time;

  float pulse = 10.0;//10.0*((sin(angle+time))+2.0)*0.02+cos(time*0.5)*0.01;

  float curRad =((sin(float(int(angle*10.0))*12.0+time*10.0)*0.1+4.0)*0.1)+(cos(float(int(angle*10.0))/10.0*50.0-time)+3.0)*0.01;
  float l=length(diff);
  vec4 outCol;
  float angleHead = 0.0;//time;

  float modV = abs(0.4*sin(7.0*time));
  float halfMod = modV / 2.0;

  if (l>(curRad-pulse)&&l < curRad && mod(angle,modV)<halfMod)
      outCol = vec4(uvRatio, 1.0,1.0);

  outCol *= drawHassle(centerUvRatio, center, vec2(1.0), angleHead);// vec4(outCol, 1.0);
  outCol.xyz += flare(diff);
  gl_FragColor = outCol;
}
