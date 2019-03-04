#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform vec2 resolution;
uniform float time;

const float cMin = 0.05;
const float cMax = 0.07;
float thick = cMax - cMin;
vec3 colTypo = vec3(0.191, 0.387, 0.734)*1.2;
vec3 colA = vec3(151.0,95.0,180.0)/255.0*1.6;
vec3 colB = vec3(240.0,149.0,55.0)/255.0*1.6;
vec3 black = vec3(0.0);
vec3 flareCol = vec3(0.123, 0.234, 0.789)*0.5;

float rand(vec2 co){ return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453); }

bool inRange(float a, float mi, float ma)
{
  return a > mi && a < ma;
}

float lengthNY(vec2 v)
{
  return abs(v.x) + abs(v.y);
}

float drawD(vec2 uv, vec2 pos, float scale)
{
  uv = (uv - pos)*(1.0/scale);
  float val = float(inRange(uv.y, -cMax, cMax) && inRange(uv.x,-thick*2.0, -thick));
  float val2 = float(inRange(length(uv), cMin, cMax) && uv.x > 0.0);
  return val + val2;
}

float drawY(vec2 uv, vec2 pos, float scale)
{
  uv = (uv - pos)*(1.0/scale);
  float valA = float(inRange(length(uv+vec2(0.0, -cMax)), cMin, cMax) && uv.y < cMax && !inRange(uv.x, -thick, thick));
  float valB = float(inRange(uv.x, -thick*0.5,thick*0.5) && inRange(uv.y, -cMax, 0.0));
  return valA +valB;
}

float drawN(vec2 uv, vec2 pos, float scale)
{
  uv = (uv - pos)*(1.0/scale);
  float val = float(inRange(length(uv), cMin, cMax) && uv.y > 0.0);
  float valB = float(inRange(uv.y, -cMax, 0.0) && (inRange(uv.x, -cMax, -cMin) || inRange(uv.x, cMin, cMax)));
  return val+valB;
}

float drawI(vec2 uv, vec2 pos, float scale)
{
  uv = (uv - pos)*(1.0/scale);
  return float(inRange(uv.y, -cMax, cMax) && inRange(uv.x, -thick*0.5,thick*0.5));
}

float drawS(vec2 uv, vec2 pos, float scale)
{
  uv = (uv - pos)*(1.0/scale);
//  float ref = float(inRange(length(uv), cMin, cMax));
  float valA = float(inRange(length(uv), cMin, cMax) && !inRange(uv.y, -thick, thick));
  float valB = float(inRange(-uv.y - uv.x*0.4, -thick*0.5, thick*0.5) &&
    length(uv) < cMax);
  return valA + valB;
}


float drawC(vec2 uv, vec2 pos, float scale)
{
  uv = (uv - pos)*(1.0/scale);
  bool onCir = inRange(length(uv), cMin, cMax);
  float valA = float(onCir && uv.x > 0.0 && !inRange(uv.y, -thick, thick) ||
    uv.x <= 0.0 && onCir);

  return valA;
}


float drawP(vec2 uv, vec2 pos, float scale)
{
  uv = (uv - pos)*(1.0/scale);
  float val = float(inRange(uv.y, -cMax, cMax) && inRange(uv.x,-thick*2.0, -thick));
  float val2 = float(inRange(length(uv+vec2(0.0, -0.02)), cMin - thick, cMin) && uv.x > 0.0);

  return val + val2;
}

float drawR(vec2 uv, vec2 pos, float scale)
{
  uv = (uv - pos)*(1.0/scale);
  float val = float(inRange(uv.y, -cMax, cMax) && inRange(uv.x,-thick*2.0, -thick));
  float val2 = float(inRange(length(uv+vec2(0.0, -0.02)), cMin - thick, cMin) && uv.x > 0.0);
  uv -= vec2(0.0, -0.02);
  float  val3 = float(inRange(-uv.y-uv.x, -thick*0.5,thick*0.5) &&
    inRange(uv.y, -0.05, -0.02));
  return val + val2 + val3;
}

float drawE(vec2 uv, vec2 pos, float scale)
{
  uv = (uv - pos)*(1.0/scale);
  bool onCir = inRange(length(uv), cMin, cMax);
  float valA = float(onCir && uv.x > 0.0 && !inRange(uv.y, -thick, thick) ||
    uv.x <= 0.0 && onCir);
  float valB = float(inRange(uv.y, -thick*0.5, thick*0.5) &&
    inRange(uv.x, -cMin + thick*0.5, cMin - thick*0.5));
  return valA+valB;
}

float drawA(vec2 uv, vec2 pos, float scale)
{
  uv = (uv - pos)*(1.0/scale);
  bool onCir = inRange(length(uv), cMin, cMax);
  float valA = float((onCir && uv.x > 0.0 && !inRange(uv.y, -thick, thick) ||
    uv.x <= 0.0 && onCir) && uv.y > 0.0);
  float valB = float(inRange(uv.y, -thick*0.5, thick*0.5) &&
    inRange(uv.x, -cMin + thick*0.5, cMin - thick*0.5));
  float valC = float(inRange(uv.x, -cMax, -cMin) && inRange(uv.y, -cMax, 0.0));
  float valD = float(inRange(uv.x, cMin, cMax) && inRange(uv.y, -cMax, -thick));
  return valA+valB+valC+valD;
}

vec3 drawTxt(vec2 uv, vec2 pos)
{
  float scale =0.4;
  float res;
  float offX = -0.11;
  float sp =1.3;

  uv.y *= (sin(10.0*uv.x+time)+1.5)*0.5;

  res += drawD(uv, pos+vec2(sp*-0.1+offX,0.0), scale);
  res += drawI(uv, pos+vec2(sp*-0.07+offX,0.0), scale);
  res += drawS(uv, pos+vec2(sp*-0.04+offX,0.0), scale);
  res += drawC(uv, pos+vec2(offX,0.0), scale);
  res += drawR(uv, pos+vec2(sp*0.037+offX,0.0), scale);
  res += drawE(uv, pos+vec2(sp*0.075+offX,0.0), scale);
  res += drawP(uv, pos+vec2(sp*0.11+offX,0.0), scale);
  res += drawA(uv, pos+vec2(sp*0.15+offX,0.0), scale);
  res += drawN(uv, pos+vec2(sp*0.195+offX,0.0), scale);
  res += drawC(uv, pos+vec2(sp*0.235+offX,0.0), scale);
  res += drawY(uv, pos+vec2(sp*0.275+offX,0.0), scale);

  return vec3(clamp(res,0.0,1.0));
}
float sat(float a)
{
  return max(min(a, 1.0),0.0);
}
vec3 drawTxtChroma(vec2 uv, vec2 pos)
{
  float spread= 0.002;
  vec2 dir=vec2(1.0,0.0);
  vec3 col;
  col.r = drawTxt(uv+dir*spread, pos).r;
  col.g = drawTxt(uv, pos).g;
  col.b = drawTxt(uv-dir*spread, pos).b;
  return col+sat(rand(uv+time))*0.8;
}


vec3 renderBack(vec2 uv, float uvYO)
{
  if (length(uv) > 0.4)
     return black;
  vec3 grad = mix(colA, colB, uvYO);

  float val = 0.005*pow(1.0-uvYO,2.0)+(sin(time+uv.x*10.0)+1.0)*0.0001;
  float modV = val *2.0;
  float coefGrnd = float(uv.y > 0.0) + uv.y+float(uv.y < 0.0 && mod(uv.y, modV)<val);
  return mix(mix(grad, black,0.1-uv.y*4.0), grad, coefGrnd);
}

vec3 render(vec2 uv, float uvYO)
{
  return renderBack(uv, uvYO);
}

vec3 rdr(vec2 uv, float buUvY)
{
  vec3 col = render(uv, buUvY);
  col=max(col, vec3(0.0));
  int samples = 2;
  vec3 acc;

  for (int i=0;i < samples;++i)
  {
    float iF = float(i - samples/2);
    iF /= float(samples/2);
    vec2 off = (vec2(rand(uv),rand(uv.yx))-0.5)*0.05;
   iF = (1.0-iF)*(length(off)+0.4);
    acc+=render(uv+off, buUvY)*iF;

  }
  return col+acc;
}

vec3 rdrChroma(vec2 uv, float uvY)
{
  vec3 col;
  float sa = sin(time*13.0);
  float sb = sin(time*13.0+0.1);
  float beat = (sa > sb ? 1.0 : 0.0)*(sa+1.0);
  float spread= 0.01*beat;
  vec2 dir=vec2(1.0,0.0);
  col.r = rdr(uv+dir*spread, uvY).r;
  col.g = rdr(uv, uvY).g;
  col.b = rdr(uv-dir*spread, uvY).b;
  return col;
}

vec3 rdrDiscrepancy(vec2 uv)
{
  vec3 col = drawTxtChroma(uv, vec2(0.0))*colTypo;
  if (col.r>0.1)
    return col;
  return vec3(0.0);
}

vec3 rdrFlare(vec2 uv)
{
  float ln = lengthNY(vec2(uv.x+sin(time*0.5)*5.0,uv.y));
  float flare = (1.0-(ln / 3.0));

  float flareB = 1.0-(pow(ln,2.0)/2.0);
  return flareCol* flare+max(flareB,0.0)*0.5;
}

void main(void) {
  vec2 uv = gl_FragCoord.xy / resolution.xy;
  float buUvY = uv.y;

  float ratio = resolution.x / resolution.y;
  uv.x *= ratio;
  uv -= vec2(0.5 * ratio, 0.5);

  vec3 outCol = rdrChroma(uv, buUvY);

  vec3 discCol = rdrDiscrepancy(uv);

  if(discCol.r > 0.05)
  {
    outCol = discCol;
    if (uv.y < 0.0 && mod(uv.y,0.005) < 0.0025)
      outCol = black;
  }
  outCol += rdrFlare(uv);
  gl_FragColor = vec4(outCol, 1.0);
}
