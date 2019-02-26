#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform vec2 resolution;
uniform float time;
uniform sampler2D SkySf;

const float PI = 3.1415927;

const vec4 hlf = vec4(0.5);
const vec4 white = vec4(1.0);
const vec4 black = vec4(0.0);
const vec4 red = vec4(1.0,0.0,0.0,0.0);
const vec4 green = vec4(0.0,1.0,0.0,0.0);
const vec4 blue = vec4(0.0,0.0,1.0,0.0);

float lengthNY(vec3 v)
{
  return abs(v.x)+abs(v.y)+abs(v.z);
}

float lengthSqr(vec3 a)
{
  return a.x*a.x+a.y*a.y+a.z*a.z;
}

float sat(float a)
{
  return clamp(a, 0.0, 1.0);
}

vec3 sat(vec3 v)
{
  return vec3(sat(v.x), sat(v.y), sat(v.z));
}

vec2 dirToUv(vec3 dir, float offYaw)
{
  float borderUp = 0.2507;
  float yaw = atan(dir.x, dir.z)/PI;
  yaw *= 0.5; // remap to 0-1
  yaw += 0.5;
  float pitch = acos(dir.y)/PI;
  float ratioY = 1.0-borderUp*2.0;
  return vec2(yaw, (1.0-pitch)*ratioY+borderUp);
}

vec3 rdrStar(vec2 angles)
{
  return vec3(0.0);
}

vec3 rdrSky(vec3 v)
{
  return vec3(0.0);
}

vec3 setCam(vec2 uv, float yaw, float pitch, float roll)
{
  vec3 fwd = vec3(0.0,0.0,1.0);
  vec3 right = vec3(-1.0,0.0,0.0);
  vec3 up = vec3(0.0,1.0,0.0);
  float fov = 15.0;
  float cosRoll = cos(roll);
  float sinRoll = sin(roll);
  float cosYaw = cos(yaw);
  float sinYaw = sin(yaw);
  float cosPitch = cos(pitch);
  float sinPitch = sin(pitch);

  mat3 rotZ = mat3(1.0, 0.0, 0.0,//pitch
    0.0,cosPitch, -sinPitch,
    0.0,sinPitch, cosPitch);
  mat3 rotY = mat3(cosYaw,  0.0, sinYaw,//yaw
                   0.0, 1.0, 0.0,
                   -sinYaw,  0.0, cosYaw);
  mat3 rotX = mat3(cosRoll, -sinRoll, 0.0,//roll
                   sinRoll, cosRoll, 0.0,
                   0.0,  0.0, 1.0);
  vec3 vec = fwd;
  vec += (uv.x*right+uv.y*up)*fov;
  vec =rotY* rotX * (rotZ* vec);
  vec= normalize(vec);

  return vec;
}

vec3 rdr(vec2 uv)
{
  vec3 orig = vec3(0.0,0.0,0.0);
  vec3 dir = setCam(uv, time*0.5, cos(time)*0.3, cos(time)*0.3);
  vec2 uv360 = dirToUv(dir, 0.0);
  vec3 tex = texture2D(SkySf, uv360).xyz;
  return tex;
}

vec3 rdrChroma(vec2 uv)
{
  vec2 dir = vec2(0.1)*sin(uv.y+time);
  float r = rdr(uv+dir).x;
  float g = rdr(uv).y;
  float b = rdr(uv-dir).z;
  return vec3(r, g , b);
}

void main(void) {
  vec2 uv = gl_FragCoord.xy / resolution.xy;
  float ratio = resolution.x / resolution.y;
  uv -= hlf.xy;
  uv.x *= ratio;

  vec3 outCol = rdrChroma(uv);
  gl_FragColor = vec4(outCol, 1.0);
}
