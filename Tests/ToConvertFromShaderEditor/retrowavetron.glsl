#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform vec2 resolution;
uniform float time;
uniform sampler2D noise;
uniform sampler2D BikeTex;
uniform vec3 orientation;
uniform vec3 rotation;
uniform sampler2D Hassle;
uniform sampler2D backbuffer;

vec3 topColor = vec3(0.945, 0.694, 0.274);
vec3 downColor = vec3(0.945, 0.274, 0.654);
float sunSize= 0.4;
float glowSize = 0.1;

const int glossSamples = 16;
float coneAperture = 0.01;

int marchStep = 512;
float marchDist = 200.0;

float EPSILON = 0.00001;


float rand(vec2 co){ return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453); }

vec3 colorlerp(vec3 a, vec3 b, float c)
{
  return a * c+ b*(1.0-c);
}
struct plane {
  vec3 pos;
  vec3 normal;
  vec3 fwd;
  int isreflect;
  vec3 spec_col;
  bool isbike;
  };

vec3 raytrace_plane(plane pl, vec3 camOrigin, vec3 vec)
{
  float dist = dot(pl.pos - camOrigin, pl.normal) / dot(vec, pl.normal);
  vec3 rightVec = cross(pl.normal, pl.fwd);
  vec3 inter = camOrigin + vec * dist;
  vec3 planeVec = inter - pl.pos;
  float u = dot(planeVec, pl.fwd);
  float v = dot(planeVec, rightVec);
  return vec3(u, v, dist); // u, v, dist
}


const int PLANECNT = 3;
plane planes[PLANECNT];

vec3 renderSky(vec2 uv)
{
  float coef = 100.0;
  vec3 stars = vec3(1.0-(min(rand(gl_FragCoord.xy*0.001),1.0/coef) *coef));
  vec3 skyCol = colorlerp(vec3(0.0),vec3(0.2,0.05,0.6), uv.y);
return skyCol + stars*(uv.y+0.4);
}

vec3 renderSun(float vCenter, vec2 uvRatio)
{
  vec3 outCol;
  float vTop = vCenter-sunSize;
  float vDown = vCenter - sunSize;

  float vCoef = max(uvRatio.y - vTop,0.0);

  float dist = length(vec2(0.5, vCenter)-uvRatio);
  vec3 sunCol = colorlerp(topColor, downColor, vCoef);
  if (dist < sunSize &&
    (vCoef >vCenter || ( vCoef <vCenter && mod(uvRatio.y,0.05) <0.02)))
  {
      outCol = vec3(sunCol);
  }
  else
   {
     outCol = renderSky(uvRatio) * 0.5;
   }
  if (dist < (sunSize + glowSize))
  {
    float glowCoef = dist / (sunSize + glowSize);
    glowCoef = glowCoef * glowCoef;
      outCol += vec3(sunCol * (1.0 - glowCoef));
  }
  float strokeThick = 0.05;
  const int strokeCount = 5;
  for (int i = 0; i < strokeCount; ++i)
  {
  }
  return outCol;
}

vec2 distortUV(vec2 uv, float distort)
{
  vec2 cosAlignedUv = (uv-vec2(0.5))*3.14159;

  float cosPI_2 = cos(cosAlignedUv.x);
  float coefDist = (1.0-((cosPI_2+1.0)/2.0));
  coefDist = pow(cosAlignedUv.x/(3.14159*0.5),2.0);
  uv.y = uv.y - coefDist*distort*(-cosAlignedUv.y);
  return uv;
}
const float PI = 3.1415927;
float clamp01(float a)
{
  return clamp(a, 0.0,1.0);
}
vec3 rdrFlare(vec2 uv)
{
  float a = atan(uv.y, uv.x)/3.1415927;
  float wave = (sin(a*PI)*cos(a*PI*20.0+time*10.0)*sin((a*PI)*50.0))*0.5+0.5;
  float l = length(uv);
  float minL = 0.3;
  float maxL = 1.0;
  //l = maxL - l;
  float op = (l - minL) / (maxL-minL);
  op = (op)*op*op;
  float opa = sin(time*12.24)*0.2+0.6;
  return vec3(wave)*clamp01(op)*opa*vec3(0.792,0.123,0.543);
}

void main(void) {
  float sinTime = sin(time);
  float cosTime = cos(time);
  vec2 uv = gl_FragCoord.xy / resolution.xy;
  vec2 uvO = uv;
  //uv.x += sin((uv.y+time)*850.0*(sin(time)*0.3+1.0))*0.02+sin(932.3*uv.y+time-0.5)*0.05;
  uv = distortUV(uv, 0.01);
  float ratio = resolution.y/ resolution.x;
  float vCenter = 0.5 * ratio + sin(time) *0.2;
  vec2 uvRatio = vec2(uv.x, uv.y * ratio);

  vec3 origin = vec3(50.0,10.0,-75.0);
  float yaw = cosTime * 0.1;//mod(iTime * 0.1, 2.0*3.1415) - 3.1415;
    float pitch = 3.1415/2.0+sinTime *0.1;
//float yaw = orientation.y+1.5;
//float pitch = rotation.z;
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
    vec3 vec = vec3(1.0, 0.0, 0.0);
    vec3 rightVec = vec3(0.0,0.0,-1.0);//cross(cam.Vec, vec3(0.0f,1.0f,0.0f));
    vec3 upVec = vec3(0.0,1.0,0.0);//cross(rightVec, vec3(1.0f,0.0f,0.0f));

    float fov = 1.5;
    vec2 centeredUv = uv.xy - vec2(0.5,0.5);
    centeredUv.y = centeredUv.y * (resolution.y / resolution.x);
    centeredUv *= fov;
    vec += centeredUv.x * rightVec + centeredUv.y * upVec;
  vec = rotY * (rotZ * vec);
  vec= normalize(vec);

  // ground
  planes[0].pos = vec3(0.0, 0.0,0.0);
  planes[0].normal = vec3(0.0,1.0,0.0);
  planes[0].fwd = vec3(0.0,0.0,1.0);
  planes[0].spec_col= vec3(1.0,0.0,0.0);
  planes[0].isreflect = 1;
  planes[0].isbike = false;

  // wall
  planes[1].pos = vec3(0.0,sinTime * 10.0 - 2.0,0.0);
  planes[1].normal = vec3(0.0,0.0,1.0);
  planes[1].fwd = vec3(1.0,0.0,0.0);
  planes[1].spec_col = vec3(0.7, cos(time/1.5)*0.5+0.5, 0.75);
  planes[1].isreflect = 0;
  planes[1].isbike = false;

  // bike
  planes[2].pos = vec3(0.0,0.0,-50.0+sinTime*10.0);
  planes[2].normal = vec3(0.0,0.0,1.0);
  planes[2].fwd = vec3(1.0,0.0,0.0);
  planes[2].spec_col = vec3(0.7, cos(time/1.5)*0.5+0.5, 0.75);
  planes[2].isreflect = 0;
  planes[2].isbike = true;

  vec3 outCol;
  float dist = -1.0;
  plane hitPlane;
  vec2 retUV;

  for(int i = 0; i < PLANECNT; ++i)
  {
    vec3 ret = raytrace_plane(planes[i], origin, vec);
    bool isTrail = (ret.y>1.0&&ret.y<(4.5-(uv.x*5.0)+sin(uv.x*20.0+time*10.0))&&ret.x<50.0&&
         length(ret.xy-vec2(50.0,2.57))>5.0);
    if (ret.z > 0.0 && (dist < 0.0 || ret.z < dist))
    {
      if (!planes[i].isbike || (planes[i].isbike &&
        ret.x > 45.0 && ret.x< 50.0 && ret.y >0.0 && ret.y < 5.0 &&
      //  texture2D(Hassle, ret.xy*0.2*vec2(-1.0,1.0)).g < 0.9))
       texture2D(BikeTex, ret.xy*0.2*vec2(-1.0,1.0)).x < 0.1)
       ||isTrail)
        {
      dist = ret.z;
      retUV = ret.xy;
      hitPlane = planes[i];
      if(isTrail)
      {
        float m = sin(((uv.x)+uv.y)*50.0+time*20.0);
        outCol = vec3(clamp01(m))+texture2D(backbuffer, uv).xyz
        *vec3(1.9,0.6,0.83);
       }
      }
    }
  }
 if (dist >0.0) {

    if(hitPlane.isreflect == 1)
    {
      vec3 accCol;
      float acc =0.0;
      vec3 waveVec = vec3(sin(time + retUV.x) *0.1,0.0,0.0);
      vec3 reflVec = normalize(reflect(vec, hitPlane.normal) + waveVec);
      vec3 inter = origin + vec * dist + hitPlane.normal *0.001;

      for (int j =0; j < glossSamples; ++j)
      {
        float blurSphereDist = 3.0;
        vec3 randBlurVec = blurSphereDist *reflVec + coneAperture *normalize(vec3(rand(inter.xy)-0.5, rand(inter.xy +1.0)-0.5,rand(inter.xy -1.0)-0.5));
        reflVec = normalize(randBlurVec);
        vec3 retRefl = raytrace_plane(planes[1], inter, reflVec);
        if (retRefl.z > 0.0)
        {
          accCol += renderSun(0.5,retRefl.xy * 0.01);
          ++acc;
        }
      }
      outCol = accCol / float(acc);
      outCol *= planes[1].spec_col;
      vec3 gridCol =vec3(0.2,0.05,0.6);
      vec2 gridSpace = vec2(2.0);
      vec2 gridWeight = vec2(0.2);
      if (mod(retUV.x, gridSpace.x)>(gridSpace.x-gridWeight.x)||
        mod(retUV.y+time*15.0, gridSpace.y)>(gridSpace.y-gridWeight.y))
        {
         outCol += gridCol/(dist*0.1);
        }
    }
    else if (hitPlane.isbike)
    {
       //outCol = texture2D(Hassle, retUV*0.2*vec2(-1.0,1.0)).rgb;
    }
    else
    {
       outCol = renderSun(0.5,retUV * 0.01);
       //outCol=vec3(texture(noise, retUV),0.0);
    }
 }
 vec3 flare = rdrFlare(centeredUv);
outCol*= flare+vec3(sin(time)*0.2+0.3,0.8,0.8);
outCol += flare*0.3;

 outCol += vec3(rand(uvRatio*time))*0.2;

 float crush = 1.0;
  float frange = 2.5;
  float exposure = 9.0;

  outCol = smoothstep(crush,frange+crush, log2(1.0+outCol*exposure));
  outCol += texture2D(backbuffer, uvO+vec2(0.02,0.0)).xyz*0.2;
  gl_FragColor = vec4(outCol,1.0);
}
