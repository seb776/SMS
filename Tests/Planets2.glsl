// TODO
// - add star background
// - god rays
// - sun rendering
// - atmospheric scattering (partially made)
//
//

#define POW2(x) ((x) * (x))
#define OFFSET_RAY 0.01

float lerp(float a, float b, float i)
{
    return (a * i) + (1.0 - i) * b;
}

vec3 lerp(vec3 a, vec3 b, float i)
{
    return (a * i) + (1.0 - i) * b;
}

struct s_planet
{
    float TrajectoryRadius;
    float Radius;
    float Pitch; // From -pi/2 to pi/2
    float Yaw; // any value
    float YawSpeed; // any value
    vec3 position;
    vec3 color;
    float atmosphericDepth;
};

struct Ray
{
    vec3 Origin;
    vec3 Vec;
};

struct Intersection
{
    Ray Ray;
    vec3 Intersection;
    float dist;
    s_planet Planet;
    bool Intersected;
    vec3 Normal;
};

Intersection RayCast(Ray ray, s_planet planet, bool hitAtmos)
{
    Intersection inter;
    inter.Intersected = false;
    vec3 oc = ray.Origin - planet.position;
	float front = -dot(ray.Vec, oc);
    float radiusSqr = 0.0;
    if (hitAtmos)
        radiusSqr = POW2(planet.Radius + planet.atmosphericDepth);
    else
        radiusSqr = POW2(planet.Radius);
	float underSqrt = POW2(dot(ray.Vec, oc)) - (length(oc) * length(oc)) + radiusSqr;
	if (underSqrt < 0.0)
		return inter;

	float dA = front - sqrt(underSqrt);
	float dB = front + sqrt(underSqrt);
			
    if (dA < 0.0 && dB < 0.0)
        return inter;
    inter.Ray.Vec = ray.Vec;
	inter.Ray.Origin = ray.Origin;
    inter.Intersected = true;
    inter.Planet = planet;
    if (dA < 0.0)
    {
		inter.dist = dB;
    }
    else
        inter.dist = dA; 
    return inter;
}

vec3 calcNormal(s_planet planet, vec3 vec, vec3 inter)
{
    			vec3 normal = normalize(planet.position - inter);

			if (dot(vec, normal) > 0.0)
				normal = -normal;
    return normal;
}

const int planetCount = 4;
const vec3 ambientLight = vec3(1.274, 1.419, 2.466);

Intersection GetIntersection(Ray cam, s_planet planets[planetCount], bool hitAtmos)
{
    bool isFirst = true;
    Intersection retainedInter;
    
    for (int i = 0; i < planetCount; ++i)
    {
        Intersection newInter = RayCast(cam, planets[i], hitAtmos);
        if (newInter.Intersected)
        {
        	if (isFirst || (!isFirst && newInter.dist < retainedInter.dist))
        	{
            	retainedInter = newInter;
                retainedInter.Intersection = cam.Origin + cam.Vec * retainedInter.dist;
            	isFirst = false;
        	}
        }
    }
    retainedInter.Normal = calcNormal(retainedInter.Planet, cam.Vec, retainedInter.Intersection);
    return retainedInter;
}

vec4 Render(Ray cam, s_planet planets[planetCount])
{
    vec3 lightVec = vec3(0.0, 0.0, 0.0); // position
    bool isFirst = true;
    vec3 baseColor = vec3(0.0,0.0,0.0);

    Intersection planetInter = GetIntersection(cam, planets, false);
    Intersection retainedInter = GetIntersection(cam, planets, true);

    if (!retainedInter.Intersected && !planetInter.Intersected) // if we didn't hit anything
    {
        return vec4(0.0, 0.0, 0.0, 0.0);
    }

    
    if (planetInter.Intersected && planetInter.Planet.TrajectoryRadius < 1.0)
    {
        vec3 sunTopColor = vec3(1.0, 0.745, 0.341);
        vec3 sunBottomColor = vec3(0.901, 0.2, 0.482);
        float topHeight = planetInter.Planet.position.y + planetInter.Planet.Radius;
        float bottomHeight = planetInter.Planet.position.y - planetInter.Planet.Radius;

        float coef = (planetInter.Intersection.y - bottomHeight) / (topHeight - bottomHeight);

        baseColor = lerp(sunTopColor, sunBottomColor, coef);


    }

    Ray newRayScatter;
    newRayScatter.Origin = retainedInter.Intersection + normalize(cam.Vec) * OFFSET_RAY;
    newRayScatter.Vec = cam.Vec;
    Intersection scatterPlanetInter = GetIntersection(newRayScatter, planets, false);
    vec3 atmosLight = vec3(0.0,0.0,0.0);
    if (scatterPlanetInter.Intersected) // Inner scatter
        {
            // Ray rayToLight;
            // rayToLight.Origin = scatterPlanetInter.Intersection;
            // rayToLight.Vec = normalize(rayToLight.Origin - lightVec);
            // GetIntersection
            float minInnerScatter = 0.0;
            float lambert = dot(normalize(lightVec - scatterPlanetInter.Intersection), normalize(scatterPlanetInter.Normal));
            float finalLambert = clamp(lambert + 0.0, minInnerScatter, 1.0);
            finalLambert = sqrt(finalLambert);
            finalLambert = saturate(finalLambert);
            float minCoefGradient = 0.1;
            float coefGradient = ((1.0 - clamp(abs(dot(newRayScatter.Vec, normalize(scatterPlanetInter.Normal))), 0.0, 1.0))) * (1.0 - minCoefGradient) + minCoefGradient;
            atmosLight = vec3(finalLambert  * coefGradient + (coefGradient * 0.1));
        }
        else // Outer scatter
        {
            Intersection scatterAtmosInter = GetIntersection(newRayScatter, planets, true);
    
            if (scatterAtmosInter.Intersected)
            {

                float coefScatter = scatterAtmosInter.dist / (scatterAtmosInter.Planet.atmosphericDepth * 8.0);
                coefScatter = saturate(coefScatter);
            
                coefScatter = POW2(POW2(coefScatter));
                float coefGradient = (1.0 - dot(newRayScatter.Vec, normalize(scatterAtmosInter.Normal)));
                float coefLambert = 1.0 - dot(normalize(lightVec - scatterAtmosInter.Intersection), normalize(scatterAtmosInter.Normal));
                atmosLight = vec3(coefScatter) * coefGradient * coefLambert * 15.9;
            }
        }

    vec3 lightColor = vec3(2.0,2.0,2.0);
    
    float coef = dot(normalize(lightVec - planetInter.Intersection), normalize(planetInter.Normal));
    //coef = coef * coef * coef * coef * coef;
    if (!planetInter.Intersected)
        return vec4(atmosLight * retainedInter.Planet.color, 1.0);
    return vec4(baseColor + (atmosLight) + coef * lightColor * planetInter.Planet.color + planetInter.Planet.color * ambientLight ,1.0);
}

//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : stegu
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//               https://github.com/stegu/webgl-noise
// 
vec3 mod289(vec3 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec2 mod289(vec2 x) {
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}

vec3 permute(vec3 x) {
  return mod289(((x*34.0)+1.0)*x);
}

float snoise(vec2 v)
  {
  const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626,  // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;

// Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
		+ i.x + vec3(0.0, i1.x, 1.0 ));

  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}

void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = gl_FragCoord.xy / iResolution.xy;
    
    vec3 lightCoordinate = vec3(0,0,0);
    s_planet planetRadii[planetCount];
    
    planetRadii[0] = s_planet(100.0, 5.0, 0.0, 0.0, 0.8, vec3(0.0,0.0,0.0), vec3(0.1,0.05,0.1), 1.2);
    planetRadii[1] = s_planet(50.0, 15.0, 0.0, 0.0, 1.0, vec3(0.0,0.0,0.0), vec3(0.1,0.1,0.15), 3.8);
    planetRadii[2] = s_planet(75.0, 7.0, 0.0, 0.0, 1.1, vec3(0.0,0.0,0.0), vec3(0.0,0.15,0.20), 2.2);
    planetRadii[3] = s_planet(0.0, 20.0, 0.0, 0.0, 1.1, vec3(0.0,0.0,0.0), vec3(0.0,0.15,0.20), 2.2);

    for (int i = 0; i < planetCount; ++i)
    {
    	float angle = mod(iGlobalTime * planetRadii[i].YawSpeed, 2.0*3.1415) - 3.1415;
 		planetRadii[i].position = vec3(planetRadii[i].TrajectoryRadius * cos(angle),  0.0,  planetRadii[i].TrajectoryRadius * sin(angle));
    }
    //float camPhi = 3.1415 / 2.0 + 0.5; // Vertical (0-180)
    //float camTheta = 0.0; // Horizontal (0-360)
    Ray cam;
    cam.Origin = vec3(-150.0,0.0 * 0.1,0.0);
    //cam.Vec = vec3(sin(camTheta) * cos(camPhi), sin(camTheta) * sin(camPhi), cos(camTheta));
    
    float yaw =0.0;
    float pitch = 0.0;//iGlobalTime * 0.1;// 0.0;// mod(iGlobalTime * 1.1, 3.1415);
    
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

    float fov = 1.5;
    vec2 centeredUv = uv - vec2(0.5,0.5);
    centeredUv.y = centeredUv.y * (iResolution.y / iResolution.x);
    centeredUv *= fov;

    const int AAsampleCount = 5;
    vec2 aaRightOffset = vec2((1.0 / iResolution.x) * 0.5, 0.0);
    vec2 aaUpOffset = vec2(0.0, ((iResolution.y / iResolution.x) / iResolution.y) * 0.5);
    vec2 AAOffsets[AAsampleCount]; 

        AAOffsets[0] = -aaRightOffset + aaUpOffset;
        AAOffsets[1] = aaRightOffset + aaUpOffset;
        AAOffsets[2] = vec2(0.0,0.0);
        AAOffsets[3] = -aaRightOffset - aaUpOffset;
        AAOffsets[4] = aaRightOffset - aaUpOffset;
    
    
    vec4 colAcc = vec4(0.0,0.0,0.0,1.0);
    for (int i = 0; i < AAsampleCount; ++i)
    {
    cam.Vec = vec3(1.0, 0.0, 0.0);

    vec3 rightVec = vec3(0.0,0.0,-1.0);
    vec3 upVec = vec3(0.0,1.0,0.0);



    cam.Vec += (centeredUv.x + AAOffsets[i].x) * rightVec + (centeredUv.y + AAOffsets[i].y) * upVec;
	cam.Vec = rotZ * (rotY * cam.Vec);
    cam.Vec = normalize(cam.Vec);
    cam.Origin += cam.Vec;

        colAcc += Render(cam, planetRadii);
    }
    vec4 colr = Render(cam, planetRadii);
    colAcc /= float(AAsampleCount);

    gl_FragColor = colAcc;// + snoise(uv * 5000000.0) * snoise(50000944.35 * vec2(uv.y, uv.x) * mod(iGlobalTime, 1.0 / iGlobalTime)) * 0.2;


    cam.Vec = normalize(cam.Vec);
    vec2 angleUv = vec2(acos(cam.Vec.z), atan(cam.Vec.y / cam.Vec.x));
    float noiseStar = clamp(snoise(angleUv), 0.8, 1.0) * snoise(angleUv * 5000000.0) * snoise(5000094.35 * vec2(angleUv.y, angleUv.x)) * 0.2;
    if (noiseStar > 0.1)
        noiseStar = 1.0;
    else
        noiseStar = 0.0;
    if (noiseStar > 0.5 && gl_FragColor.x < 0.01 && gl_FragColor.y < 0.01 && gl_FragColor.z < 0.01)
           gl_FragColor = vec4(noiseStar);
    // Time varying pixel color
    //vec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));

    // Output to screen
    //fragColor = vec4(col,1.0);
}