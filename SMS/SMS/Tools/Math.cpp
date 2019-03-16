#include "Math.h"

float fmod(float a, float b)
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

constexpr int fact(int n) {
	return n ? (n * fact(n - 1)) : 1;
}

float MySin(float a)
{
	a = fmod(a, SMS_PI_F);
	return a - (POW3(a) / (float)fact(3)) + (POW5(a) / (float)fact(5)) - (POW7(a) / (float)fact(7));
}
