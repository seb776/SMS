#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform vec2 resolution;
uniform float time;

const vec4 hlf = vec4(0.5);

const int marchStp = 64;
const float maxDst = 150.0;
const float EPS = 0.000001;
float lengthNY(vec3 v)
{
  return abs(v.x)+abs(v.y)+abs(v.z);
}
float lengthSqr(vec3 a)
{
  return a.x*a.x+a.y*a.y+a.z*a.z;
}

vec3 rdrStar(vec2 angles)
{
  return vec3(0.0);
}
float sat(float a)
{
  return clamp(a,0.0,1.0);
}
vec3 rdrSky(vec3 v)
{
  return pow(sat(1.05-abs(v.y)),5.0)*vec3(0.4,0.5,0.976)*0.5;
  return vec3(0.0);
}
vec3 colRainbowA = vec3(69,131,211) / 255.0;
vec3 colRainbowB = vec3(44,232,56) / 255.0;
vec3 colRainbowC = vec3(249,173,79) / 255.0;
vec3 colRainbowD = vec3(224,83,17)  / 255.0;

vec3 rdrSky2(vec3 v)
{

  // 0 - 1 // 
  float maxVal = 3.0;
  float f = abs(v.y) * 8.0;
  if (f > maxVal)
    return vec3(0.0);
  if (mod((sin(time) + 1.5) * 4.0, f) < 1.0)//f < (maxVal / 4.0))
    return 1.7*mix(colRainbowD*1.2+vec3(0.8), colRainbowC, f / (maxVal / 4.0));
  //return 

  return pow(sat(1.05-abs(v.y)),5.0)*vec3(0.4,0.5,0.976)*1.5;
  return vec3(0.0);
}



float sdf_cube(vec3 p)
{
    vec3 sz = vec3(0.5);
    vec3 diff = abs(p) - sz;

    return max(diff.x, max(diff.y, diff.z));
    return 0.0;
}

float sdf_sph(vec3 p, float rad)
{
  return length(p)-rad;
}

vec3 sdf_repeat(vec3 p, vec3 c)
{
  return mod(p,c)-0.5*c;//+vec3(sin(time+p.y*3.0))*0.1;
}

vec3 setCam(vec3 origin, vec2 uv)
{
  float pitch = cos(0.1*time)*0.9+0.39;
  float yaw = sin(time*0.2)*0.8;
  vec3 fwd = vec3(0.0,0.0,1.0);
  vec3 right = vec3(-1.0,0.0,0.0);
  vec3 up = vec3(0.0,1.0,0.0);
  float fov = 1.0;
  float cosYaw = cos(yaw);
  float sinYaw = sin(yaw);
  float cosPitch = cos(pitch);
  float sinPitch = sin(pitch);

  mat3 rotY = mat3(cosPitch,  0.0, sinPitch,
                   0.0, 1.0, 0.0,
                   -sinPitch,  0.0, cosPitch);
  mat3 rotZ = mat3(cosYaw, -sinYaw, 0.0,
                   sinYaw, cosYaw, 0.0,
                   0.0,  0.0, 1.0);
  vec3 vec = fwd;
  vec += (uv.x*right+uv.y*up)*fov;
  vec = rotY * (rotZ * vec);
  vec= normalize(vec);

  return vec;
}
vec3 sat(vec3 v)
{
  return vec3(clamp(v.x, 0.0, 1.0), clamp(v.y, 0.0, 1.0), clamp(v.z, 0.0, 1.0));
}
vec3 rdr(vec2 uv)
{
  vec3 orig=vec3(0.0,time*0.7,-5.0);
  vec3 p;
  vec3 dir = setCam(orig, uv);
  float totDst = 0.0;
  vec3 outCol = sat(rdrSky2(dir)*0.5);

  p = orig+dir;
  for (int i = 0; i< marchStp&&totDst < maxDst&&outCol.x<0.62;++i)
  {
    //float dst = sdf_sph(sdf_repeat(p,vec3(3.0)), 0.58);
    float dst = sdf_cube(sdf_repeat(p,vec3(3.0)));

    if (dst <EPS)
    {
      outCol += 0.2*hlf.xyz*(-dst*10.0)*(maxDst-totDst)/maxDst;
      dst = -dst+0.5+EPS;

    //  break;
    }
      totDst+= dst;
      p += dir*dst;
  }
  outCol *= vec3(0.6, 0.13, 0.477) + vec3(0.3);
  return outCol;
}

vec3 rdrChroma(vec2 uv)
{
  vec2 dir = vec2(0.005);
  float r = rdr(uv+dir).x;
  float g = rdr(uv).y;
  float b = rdr(uv-dir).z;
  //return rdr(uv);
  return vec3(r,g,b);
}

void main(void) {
  vec2 uv = gl_FragCoord.xy / resolution.xy;
  float ratio = resolution.x / resolution.y;
  uv -= hlf.xy;
  uv.x *= ratio;

  vec3 outCol = rdrChroma(uv);
  gl_FragColor = vec4(outCol, 1.0);
}
