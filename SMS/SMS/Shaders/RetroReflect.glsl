uniform float iGlobalTime;
uniform vec2 iResolution;

uniform vec3 topColor = vec3(0.945, 0.694, 0.274);
uniform vec3 downColor = vec3(0.945, 0.274, 0.654);
uniform float sunSize = 0.4;
uniform float glowSize = 0.1;

uniform int glossSamples = 8;
uniform float coneAperture = 0.3;

uniform int marchStep = 512;
uniform float marchDist = 200.0;

uniform float EPSILON = 0.00001;


vec3 colorlerp(vec3 a, vec3 b, float c)
{
	return a * c + b * (1.0 - c);
}
struct plane {
	vec3 pos;
	vec3 normal;
	vec3 fwd;
	int isreflect;
	vec3 col;
};

vec3 raytrace_plane(plane pl, vec3 camOrigin, vec3 vec)
{
	float dist = dot(pl.pos - camOrigin, pl.normal) / dot(vec, pl.normal);
	vec3 rightVec = cross(pl.normal, pl.fwd);
	vec3 inter = camOrigin + vec * dist;
	vec3 planeVec = inter - pl.pos;
	float u = dot(planeVec, pl.fwd);
	float v = dot(planeVec, rightVec);
	return vec3(u, v, dist);
}


const int PLANECNT = 2;
plane planes[PLANECNT];

vec3 renderSun(float vCenter, float ratio, vec2 uvRatio)
{
	vec3 outCol;
	float vTop = vCenter - sunSize * ratio;
	float vDown = vCenter - sunSize * ratio;

	float vCoef = max(uvRatio.y - vTop, 0.0);

	float dist = length(vec2(0.5, vCenter) - uvRatio);
	vec3 sunCol = colorlerp(topColor, downColor, vCoef);
	if (dist < sunSize)
	{
		if (vCoef > 0.7 || (vCoef < 0.7 && mod(uvRatio.y, 0.05) < 0.02))
			outCol = vec3(sunCol);
	}
	if (dist < (sunSize + glowSize))
	{
		float glowCoef = dist / (sunSize + glowSize);
		glowCoef = glowCoef * glowCoef;
		outCol += vec3(sunCol * (1.0 - glowCoef));
	}
	return outCol;
}

void main(void) {
	vec2 myRes = vec2(1024.0, 768.0);
	vec2 uv = gl_FragCoord.xy / myRes.xy;
	float ratio = myRes.y / myRes.x;
	float vCenter = 0.5 * ratio + sin(0.0) *0.2;
	vec2 uvRatio = vec2(uv.x, uv.y * ratio);

	vec3 origin = vec3(0.0, 10.0, -50.0);
	float yaw = 0.0;
	float pitch = 3.1415 / 2.0 + sin(0.0) *0.1;

	float cosYaw = cos(yaw);
	float sinYaw = sin(yaw);
	float cosPitch = cos(pitch);
	float sinPitch = sin(pitch);

	mat3 rotY = mat3(cosPitch, 0.0, sinPitch,
		0.0, 1.0, 0.0,
		-sinPitch, 0.0, cosPitch);
	mat3 rotZ = mat3(cosYaw, -sinYaw, 0.0,
		sinYaw, cosYaw, 0.0,
		0.0, 0.0, 1.0);
	vec3 vec = vec3(1.0, 0.0, 0.0);
	vec3 rightVec = vec3(0.0, 0.0, -1.0);
	vec3 upVec = vec3(0.0, 1.0, 0.0);

	float fov = 1.5;
	vec2 centeredUv = uv.xy - vec2(0.5, 0.5);
	centeredUv.y = centeredUv.y * (768.0 / 1024.0);
	centeredUv *= fov;
	vec += centeredUv.x * rightVec + centeredUv.y * upVec;
	vec = rotY * (rotZ * vec);
	vec = normalize(vec);


	planes[0].pos = vec3(0.0, -abs(cos(0.0)) * 5.0 + 2.0, 0.0);
	planes[0].normal = vec3(0.0, 1.0, 0.0);
	planes[0].fwd = vec3(0.0, 0.0, 1.0);
	planes[0].col = vec3(1.0, 0.0, 0.0);
	planes[0].isreflect = 1;

	planes[1].pos = vec3(0.0);
	planes[1].normal = vec3(0.0, 0.0, 1.0);
	planes[1].fwd = vec3(1.0, 0.0, 0.0);
	planes[1].col = vec3(0.0, 0.0, 1.0);

	vec3 outCol;
	float dist = -1.0;
	plane hitPlane;
	vec2 retUV;

	for (int i = 0; i < PLANECNT; ++i)
	{
		vec3 ret = raytrace_plane(planes[i], origin, vec);

		if (ret.z > 0.0 && (dist < 0.0 || ret.z < dist))
		{
			dist = ret.z;
			retUV = ret.xy;
			hitPlane = planes[i];
		}
	}
	if (dist > 0.0) {
		if (hitPlane.isreflect == 1)
		{
			vec3 reflVec = reflect(vec, hitPlane.normal);

			vec3 inter = origin + vec * dist + hitPlane.normal * 0.001;
			const int sampleBlur = 16;
			float glossiness = 0.5;
			vec3 accCol;
			int acc = 0;

			for (int j = 0; j < sampleBlur; ++j)
			{
				vec3 reflRet = raytrace_plane(planes[1], inter, reflVec);
				if (reflRet.z > 0.0)
				{
					accCol += renderSun(0.5, 1.0, reflRet.xy * 0.05);
					++acc;
				}
			}

			outCol = accCol / float(acc);
		}
		else
		{
			outCol = renderSun(0.5, 1.0, retUV * 0.05);

		}
	}

	gl_FragColor = vec4(outCol, 1.0);
};
