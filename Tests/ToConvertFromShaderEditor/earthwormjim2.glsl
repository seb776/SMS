#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform vec2 resolution;
uniform float time;
uniform sampler2D EarthWorm;
vec3 gunCol = vec3(0.958, 0.13, 0.05);
vec3 yellow = vec3(255.0,65.0,0.0)/255.0;

float rand(vec2 co){ return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453); }

vec3 quantizeColor(vec2 uv, vec3 c, int depth)
{

  c = c+(vec3(rand(uv))-0.5)*0.2;
  int chColCnt = int(pow(2.0, float(depth)));
  float stepC = 1.0/float(chColCnt);
  c.x = float(int(c.x / stepC))*stepC;
  c.y = float(int(c.y / stepC))*stepC;
  c.z = float(int(c.z / stepC))*stepC;
  return c;
}

float lengthNY(vec2 v)
{
  return abs(v.x)+abs(v.y);
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

bool isIn(float a, float mi, float ma)
{
  return a > mi && a < ma;
}

vec4 drawSph(vec2 uv, vec3 pos, float sz, vec3 col, float light)
{
  uv -= pos.xy;
  uv *= pos.z;
  float len = length(uv);
  float c = float(len < sz);
  sz*= 4.0;
  float len2 = length(uv+vec2(0.0,-0.1*sz));
  float l = pow(max((sz-len2)/sz,0.0),4.0)*2.5;
  return vec4(c*col*l, pos.z*c);
}

vec4 drawStar(vec2 uv, vec3 pos)
{
  vec2 uvO = uv;
  uv -= pos.xy;
  uv *= pos.z;
  float maxD = 2.0;
  float len =pow(max((maxD- lengthNY(uv))/maxD,0.0),20.0);
  float inStar = float(len>0.1);
  return vec4(len,len,len, len);
}

vec4 drawGun(vec2 uv, vec3 pos)
{
  vec2 uvO = uv;
  uv -= pos.xy;
  uv *= pos.z;
  float dashL = (min(max(uv.y, -0.02), 0.02)+0.02)/0.04;
  vec3 dash = dashL*float(uv.x > -0.15 && uv.x < 0.0&&abs(uv.y)<0.02)*gunCol;
  float ell = float(length(vec2(uv.x*0.6, uv.y)) < 0.1 && uv.x  >-0.05);
  float shadow = min(float(uv.x > -0.03 && uv.x < 0.0),0.3);
  float light = pow(1.0-sqrt(uv.y-0.05),2.0)-shadow;
  float refl = pow(light, 4.0);
  vec3 sph = drawSph(uvO, pos-vec3(0.15,0.0,0.0)/pos.z, 0.04, vec3(0.854,0.7,0.0), 1.0).xyz;
  vec3 outCol = ell*gunCol*light+refl*ell*yellow+sph;
  if (length(outCol)<0.05)
    outCol = dash;
  float isHand = float(isIn(uv.y-uv.x*0.5,-0.3,-0.05)&&isIn(-uv.y-uv.x*1.9+0.2,-0.1,0.1));
  float isHndB = 1.0-((min(max((-uv.y-uv.x*1.8+0.2), -0.1), 0.1)+0.1)/0.2);
  if (length(outCol)<0.05)
    outCol = gunCol*isHand*0.4*(isHndB*isHndB*2.0+0.4);
  return vec4(outCol, pos.z*min(ell+sph+dash+isHand,1.0));
}

vec3 drawTunnel(vec2 uv)
{
  vec2 uvO = uv;
  uv += vec2(sin(time),cos(time))*0.2;
  float c = length(uvO)+mod(length(uv)-time*0.5,0.4);
  return vec3(uv,1.0)*c;
}

vec4 renderStars(vec2 uv, vec2 center)
{
  //float t = rand(uv);
  uv += vec2(sin(time),cos(time))*0.2;
  vec4 s;
  const int starCnt = 15;
  for(int i = 0;i<starCnt;++i)
  {
    float fi = float(i);
    float r = mod(sqrt((fi/float(starCnt))*0.2)+time*0.2,0.5);
    vec2 pos = vec2(r*cos(fi),r*sin(fi));
    s += drawStar(uv, vec3(pos.x, pos.y, sin(time*0.48294*fi)+fi+1.0))*sqrt(r*5.0);
  }

  return s;//vec4(1.0,1.0,1.0, float(isIn(t,0.199,0.2)));
}

vec3 render(vec2 uv)
{
  vec3 outCol = drawTunnel(uv);
  const int oCnt=2;
  vec4 objs[oCnt];
  int hit = -1;

  vec3 gunPos = vec3(0.0,cos(time)*0.2,(sin(time)+2.5));
  objs[0] = drawGun(uv, gunPos);

  for(int i = 0; i<oCnt;++i)
  {
    if (objs[i].w > 0.1)
    {
      if(hit!=-1 && objs[i].w < objs[hit].w)
        hit = i;
      else if(hit==-1)
        hit = i;
    }
  }
  if(hit !=-1)
  {
      outCol = objs[hit].xyz;
  }
  vec4 stars = renderStars(uv, uv);
  outCol += drawStar(uv, gunPos+vec3(0.0,0.0,-1.5)).w*yellow*1.2;
  outCol += stars.xyz*stars.w;
  return outCol;
}

void main(void) {
  vec2 uv = gl_FragCoord.xy / resolution.xy;
  vec2 uvOld = uv;
  float ratio = resolution.x / resolution.y;
  uv.x *= ratio;
  uv -= vec2(0.5 * ratio, 0.5);
  vec3 outCol = vec3(uvOld, 1.0);
  uv = myPixel(uv,200.0);

  outCol = render(uv);


  outCol = quantizeColor(uv, outCol, 3);
  gl_FragColor = vec4(outCol, 1.0);
}
