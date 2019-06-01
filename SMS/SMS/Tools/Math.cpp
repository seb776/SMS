#include <SSEMaths/sse_mathfun.h>
#include <smmintrin.h>
#include "Math.h"

using namespace Discrepancy;

float Math::Lerp(float a, float b, float f)
{
	return (a + f * (b - a));
}

float Math::Fmod(float a, float b)
{
	float div = a / b;
	__m128 div4 = _mm_set_ps1(div);
	__m128 flr = _mm_floor_ps(div4);

	return a - b * flr.m128_f32[0];

	// Handling negative values 
	if (a < MathConstants::EPS)
		a = -a;
	if (b < MathConstants::EPS)
		b = -b;

	// Finding mod by repeated subtraction 
	float mod = a;
	while (mod >= b)
		mod = mod - b;

	// Sign of result typically depends 
	// on sign of a. 
	if (a < 0.0f)
		return -mod;

	return mod;
}

float Math::Abs(float a)
{
	return (a < 0.0 ? -a : a);
}

float Math::Sign(float a)
{
	return (a < 0.0 ? -1.0 : 1.0);
}

float Math::IsNeg(float a)
{
	return (a < 0.0 ? 1.0 : 0.0);
}

float Math::Sin(float a)
{
	v4sf vec = _mm_set_ps1(a);
	v4sf res = sin_ps(vec);
	return res.m128_f32[0];
	//float sign = (Math::Sign(a)*(1.0*Math::IsNeg(a)- Math::Fmod((float)(int)(Math::Abs(a) / MathConstants::PI), 2.0f))+0.5)*2.0;
	//a = Fmod(a, MathConstants::PI);
	//a = (a / MathConstants::PI)*180.0;
	//float sina = 4.0 * a * (180.0 - a) / (40500.0 - a * (180.0 - a));
	//return sign * sina;
}

float Math::Cos(float a)
{
	return Math::Sin(a + MathConstants::PI_2);
}

float Math::Fract(float a)
{
	return a - (float)(int)a;
}

float Math::Ceil(float a)
{
	return (float)((int)a + 1 * (int)(a > 0.0));
}
