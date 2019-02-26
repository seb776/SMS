#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform vec2 resolution;
uniform float time;
vec4 white = vec4(1.0);
vec4 black = vec4(0.0);

vec4 drawMount(vec2 uv, vec2 pos, vec3 colA, vec3 colB, float coef)
{
  uv-= pos;
  float mnt = sin(uv.x*50.0)*0.012+(cos(uv.x*11.35)*0.05*(sin(uv.x*22.418)+1.0));
  vec3 col = mix(colA, colB, uv.y);
  return vec4(col, float(uv.y <(mnt*coef)));
}

vec4 drawCircle(vec2 uv, vec2 pos, float r, vec3 centerCol, vec3 borderCol)
{
  uv -= pos;
  float cRad = r-length(uv);
  float inCircle = float(cRad > 0.0);
  float coef = clamp(cRad,0.0,r)/r;
  return vec4(mix(borderCol, centerCol, coef).xyz,  inCircle);
}

vec3 rdr(vec2 uv)
{
  float orbSun = 0.2;
  vec2 sunPos = vec2(orbSun * cos(time), orbSun*sin(time));
  vec3 colA = vec3(0.739, 0.32, 0.585)*0.01;
  vec3 colB = vec3(0.77,0.75,0.765);
  vec3 backCol = drawCircle(uv, sunPos, 1.0, colB, colA).xyz;
  vec4 sun = 5.0*drawCircle(uv, sunPos, 0.05, white.xyz,white.xyz*0.1);
  vec3 outCol = backCol*0.5*(0.5+sunPos.y) + sun.xyz*sun.w;

  float mntSpeed = time*0.1;
  vec4 mnt = drawMount(uv, vec2(mntSpeed,0.0), black.xyz, vec3(0.98,0.32,0.45),3.1415927);
  if (mnt.w > 0.1)
    outCol = mnt.xyz;
  mnt = drawMount(uv, vec2(1.5*mntSpeed+3.0,-0.1), black.xyz, vec3(0.98,0.32,0.45),0.531415927);
  if (mnt.w > 0.1)
    outCol = mnt.xyz;
  mnt = drawMount(uv, vec2(3.0*mntSpeed+43.023,-0.2), black.xyz, vec3(0.98,0.32,0.45),0.231415927);
  if (mnt.w > 0.1)
    outCol = mnt.xyz;
  return outCol + backCol*1.5;
}
const float PI = 3.1415927;
vec3 rdrCloud(vec2 uv, vec2 pos)
{
  uv -= pos;
  uv.x*=0.4;
  float angle = atan(uv.y, uv.x)/3.1415927;
  float rad =(0.1+0.01*sin(angle*PI*5.0)+cos(angle*3.0*PI)*0.02);
  float isCld = float(length(uv) < (rad*0.5));
  return vec3(isCld)*vec3(0.34,0.54,0.74)*2.0;
}

vec3 rdrClouds(vec2 uv)
{
  vec3 cldA = rdrCloud(uv, vec2(0.0,0.3));
  vec3 cldB = rdrCloud(uv, vec2(0.2,0.28));
  return cldA+cldB;
}

float lengthNY(vec2 v)
{
  return abs(v.x)+abs(v.y);
}

void main(void) {
  vec2 uv = gl_FragCoord.xy / resolution.xy;
  vec2 uvO = uv;

  float ratio = resolution.x/resolution.y;
  uv.x *= ratio;
  uv -= vec2(0.5*ratio, 0.2);
  uv*=1.7;
  vec3 outCol = rdr(uv);

  vec3 amb = mix(white.xyz, vec3(0.44,0.13,0.45),clamp(cos(time*0.1)*2.0,0.0,0.5));
  //outCol += rdrClouds(uv);
  float lenDark = lengthNY(uvO-vec2(0.5+0.2*cos(time),0.1+0.2*sin(time)));
  float darkRad = 0.8;
  vec4 darkCol = vec4(0.2,0.33,0.75,0.0);
  vec3 dark = mix(darkCol, white, clamp((darkRad-lenDark) / darkRad,0.0,1.0)).xyz;
  vec3 finalCol = outCol*amb*dark;
//  finalCol.y = min(finalCol.y, 0.4)*0.1;
  float crush = 0.15;
  float frange = 5.0;
  float exposure = 20.0;

  finalCol = smoothstep(crush,frange+crush, log2(1.0+finalCol*exposure));
  finalCol = finalCol*0.6+0.2;
  gl_FragColor = vec4(finalCol, 1.0);
}
