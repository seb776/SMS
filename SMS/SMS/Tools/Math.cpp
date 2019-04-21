#include "Math.h"

using namespace Discrepancy;

float Math::Fmod(float a, float b)
{
	// Handling negative values 
	if (a < 0.0f)
		a = -a;
	if (b < 0.0f)
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

	float sign = (Math::Sign(a)*(1.0*Math::IsNeg(a)- Math::Fmod((float)(int)(Math::Abs(a) / MathConstants::PI), 2.0f))+0.5)*2.0;
	a = Fmod(a, MathConstants::PI);
	a = (a / MathConstants::PI)*180.0;
	float sina = 4.0 * a * (180.0 - a) / (40500.0 - a * (180.0 - a));
	return sign * sina;
}

float Math::Cos(float a)
{
	return Math::Sin(a + MathConstants::PI_2);
}

float Math::Ceil(float a)
{
	return (float)((int)a + 1 * (int)(a > 0.0));
}
