#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform vec2 resolution;
uniform float time;

const float maxDist = 100.0;
const int maxSteps = 128;

#define EPS 0.0001

int interObj = -1;
float lengthNY(vec3 a)
{
  return pow(abs(a.x)+abs(a.y)+abs(a.z),15.0);
}
float sdf_sph(vec3 p, vec3 o, float s)
{
  return lengthNY(p-o) - s;
}

float sdf_blob(vec3 p, vec3 o, vec3 s)
{
  return length(p - o) - s.x + sin(p.x*time*5.0*p.y+p.z*10.0)*0.05;
}

float smin(float a, float b)
{
  float k = 0.5;
  float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 ); return mix( b, a, h ) - k*h*(1.0-h);
}

int imin(float a, float b)
{
  return int(a < b);
}

float sdf_scene(vec3 p)
{
  //float sphs[5]
  float spha = sdf_blob(p, vec3(0.0), vec3(1.0));
  float sphb = sdf_sph(p, vec3(1.5*sin(time*3.42),0.0,cos(time*3.42)), 0.5);
  float res = smin(spha, sphb);
  return res;
}

vec3 camLookAt(vec3 origin, vec2 uv, vec3 at)
{
  vec3 dir = normalize(at - origin);
  vec3 right = normalize(cross(dir, vec3(0.0,1.0,0.0)));
  vec3 up = normalize(cross(dir, right));
  float fov = sin(time)+2.0;
  return dir + (up * uv.y + right * uv.x) * fov;
}

vec3 renderScene(vec3 orig, vec3 dir)
{
  float totDist = -1.0;
  vec3 ite = orig;
  for (int i = 0; i < maxSteps && totDist < maxDist; ++i)
  {
     float d = sdf_scene(ite);
     if (d < EPS)
        return (mod(ite.x*ite.y+time, 0.5)<0.05?vec3(1.0,0.0,0.0):vec3(0.0,0.0,10.0*totDist/maxDist));
     ite += dir * d;
     totDist += d;
  }
  return vec3(0.0);
}

vec3 render(vec2 uv)
{
  float dist = (sin(time*10.0)+8.0)*1.0;
  vec3 origin = vec3(cos(time)*dist, 0.0, sin(time)*dist);
  vec3 dir = camLookAt(origin, uv, vec3(0.0, 0.0, 0.0));
  return renderScene(origin, dir);
}

void main(void) {
  vec2 uv = gl_FragCoord.xy / resolution.xy;
  float ratio = resolution.x / resolution.y;
  uv.x *= ratio;
  vec2 center = vec2(0.5*ratio, 0.5);
  uv-=center;
  float atten = length(uv);
  vec3 col = render(uv);
  vec2 rotUv = uv * mat2(-cos(time), sin(time),-sin(time), -cos(time));
  float starWidth = 0.3*(atten+0.3*sin(time));
  vec3 c = vec3(float(mod(atan(rotUv.y, rotUv.x), starWidth)<(starWidth /3.0))+1.0-atten);
  if (length(col) < EPS)
  {

    atten *= atten;
    vec3 colA = vec3(0.1,0.013,0.728);
    vec3 colB = vec3(0.0);
    col = vec3(uv.xy,mod(time, uv.y))+mix(colB, colA, atten) + c* vec3(uv.x+0.5,0.0,uv.y+0.5);
  }
  gl_FragColor = vec4(col, 1.0);
}
