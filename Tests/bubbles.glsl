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
  return pow(sat(1.05-abs(v.y)),5.0)*vec3(0.4,0.5,0.976)*1.5;
  return vec3(0.0);
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
  float pitch = cos(0.4*time)*0.1+0.39;
  float yaw = sin(time*0.1)*0.55;
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

vec3 rdr(vec2 uv)
{
  vec3 orig=vec3(0.0,0.1*sin(time),-5.0);
  vec3 p;
  vec3 dir = setCam(orig, uv);
  float totDst = 0.0;
  vec3 outCol = rdrSky(dir);

  p = orig+dir;
  for (int i = 0; i< marchStp&&totDst < maxDst&&outCol.x<0.62;++i)
  {
    float dst = sdf_sph(sdf_repeat(p,vec3(3.0)), 0.58);
    if (dst <EPS)
    {
      outCol += 0.1*hlf.xyz*(-dst*10.0)*(maxDst-totDst)/maxDst;
      dst = -dst+0.5+EPS;

    //  break;
    }
      totDst+= dst;
      p += dir*dst;
  }
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
