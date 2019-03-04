#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform vec2 resolution;
uniform float time;
uniform sampler2D tunnelTex;
uniform sampler2D tunnelTex2;

float rand(vec2 co){ return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453); }

struct bezier
{
  vec2 p0;
  vec2 p1;
  vec2 p2;
  vec2 p3;
};

vec2 calculateBezierPoint(bezier bz, float t)
{
  float oneMt = 1.0 - t;

  return bz.p0 * pow(oneMt,3.0) +
     3.0 *bz.p1*t*pow(oneMt,2.0) +
       3.0*bz.p2*t*t*oneMt +
         bz.p3*t*t*t;
}

vec2 myPixel(vec2 uv, float coef)
{
  float coefL = coef;//0.05;//mix(0.2, 0.012, cos(time/1.14)*0.5+0.5);
  float cx = coefL * resolution.x;
  float cy = cx *(resolution.y/ resolution.x);
  vec2 coef2d= vec2(cx, cy);
  vec2 coefed = uv *coef2d;
  return (coefed - fract(coefed))/coef;
}

vec3 tunnel(vec2 uv, float vC)
{
  float dist = 1.0/(distance(uv, vec2(0.5, vC)));
  dist = pow(dist, (1.0+sin(time))*2.0);
  uv = uv - vec2(0.5, vC);
  float a = atan(uv.y, uv.x)/3.1415927;//+sin(time*3.14)*sin(dist);
 //return vec3(abs(a));
  return texture2D(tunnelTex2, vec2(0.05*dist+time, a)).rgb;
  //return vec3(0.0);
}

void main(void) {
  vec2 uv = gl_FragCoord.xy / resolution.xy;
  float ratio = resolution.y/ resolution.x;
  float vCenter = 0.5 * ratio;// + sin(time) *0.2;
  vec2 uvRatio = vec2(uv.x, uv.y * ratio);

 vec2 sinUv = vec2(sin(uvRatio.x+uvRatio.y+time), uvRatio.y);
  vec2 uvBack = myPixel(uvRatio,abs(sinUv.x)*0.05);
  vec3 outCol = tunnel(uvBack, vCenter);

  uvRatio = myPixel(uvRatio,0.05);

  uvRatio *= 2.0;
  uvRatio -= vec2(1.0, 1.0);
  bezier bz;

  bz.p0 = vec2(cos(-time+10.0),sin(-time+10.0))*0.3;
  bz.p1 = vec2(0.5,0.0+sin(time*0.89));
  bz.p2 = vec2(0.5,1.0+sin(time+12.34));
  bz.p3 = vec2(cos(time*1.7391),1.373*sin(time+17.0)-0.5);

  float radiusLast = 0.05 + (1.0+sin((time-0.1)*14.0))*0.05;
  float radius = 0.05 + (1.0+sin(time*14.0))*0.05;

  bool isInc = radius > radiusLast;

  if (isInc)
    radius = 0.15;

  float step = 0.05;

  for (float t =0.0; t<1.0; t+=step)
  {
    vec2 p = calculateBezierPoint(bz, t);
    float dist =distance(p, uvRatio);
    float pct = 0.05;
    float rad = radius * (mix(0.2, 2.0, t));
    if (dist < rad && dist > (rad - (rad*pct)))
    outCol += vec3(uv,1.0)* max(outCol, vec3(0.5));
  }

  float coef = radius*4.0*(1.0+sin(time*0.12345));
  coef = coef*coef;

    outCol += vec3(uv,1.0)*vec3(0.1, 0.4, 0.73)*vec3(distance(uv, vec2(0.5,vCenter)))* coef;

  gl_FragColor = vec4(outCol, 1.0);
}
