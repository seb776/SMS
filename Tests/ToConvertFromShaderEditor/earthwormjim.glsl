#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform vec2 resolution;
uniform float time;
uniform sampler2D EarthWorm;

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

float drawOrb(vec2 uv, vec2 pos, float depth)
{
  float scale = 1.0;
  uv -= pos;
  float sz = 0.03;
  return float((length(uv)/depth) < sz);
}

vec4 drawAmmo(vec2 uv, vec2 pos, float depth)
{
  float scale = 1.0;
  float orbRad = 0.2;
  depth+= orbRad;
  uv -= pos;
  float inC = float(length(uv)/depth < (0.1*scale));
  float coef=1.0-(length(uv+vec2(0.02,-0.03))/0.2*scale/depth);
  coef = pow(coef,2.0)*10.0;
  vec3 cCol = inC*vec3(0.05, 0.123, 0.897)*coef;

  float orbADep = depth+sin(time*5.0)*orbRad;
  float orbA = drawOrb(uv, vec2(cos(time*5.0)*orbRad,cos(time*5.0)*orbRad), orbADep);
  vec3 orbCol = vec3(0.078, 0.75, 0.49);
  cCol *= inC;
  if (bool(inC) && bool(orbA))
  {
    cCol = (depth > orbADep?cCol:orbCol*0.3);
  }
  else if(orbA > 0.1)
    cCol = orbCol;
  inC = max(inC, orbA);
  return vec4(cCol, inC);
}


vec3 render(vec2 uv)
{
  vec3 back = vec3(uv, (sin(time)+2.0)*0.33);
  back = texture2D(EarthWorm, uv+vec2(0.5)).xyz;
  float wid= 0.1*(1.0-uv.y);
  float mov = (uv.y)*5.0+sin(uv.y*3.0+time*0.1)*12.345;
  float mov2 = sin(uv.y*10.0+time)*0.9;
  float amp = 0.05*sin(time);
  float isTree = float(isIn(uv.x+wid*0.5, sin(mov)*amp-mov2*amp, amp*sin(mov)+wid+mov2*amp));
  return back * (1.0-isTree);
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

  vec4 ammo = drawAmmo(uv,vec2(sin(time)*0.2,0.0), (cos(time)+1.0)*0.2);
  outCol = mix(outCol, ammo.xyz, ammo.w);
  outCol = quantizeColor(uv, outCol, 2);
  gl_FragColor = vec4(outCol, 1.0);
}
