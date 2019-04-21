///
/// \Brief		The Math.h header defines various mathematical functions and several macros.
/// 

#pragma once

namespace Discrepancy
{
	namespace MathConstants {
		const float E = 2.71828182845904523536f;
		const float LOG2E = 1.44269504088896340736f;
		const float LOG10E = 0.434294481903251827651f;
		const float LN2 = 0.693147180559945309417f;
		const float LN10 = 2.30258509299404568402f;
		const float PI = 3.14159265358979323846f;
		const float PI_2 = 1.57079632679489661923f;
		const float PI_4 = 0.785398163397448309616f;
		const float PI_1_PI = 0.318309886183790671538f;
		const float PI_2_PI = 0.636619772367581343076f;
		const float PI_2_SQRTPI = 1.12837916709551257390f;
		const float SQRT2 = 1.41421356237309504880f;
		const float SQRT1_2 = 0.707106781186547524401f;
	};

	namespace Math
	{
		constexpr float Pow(float a, unsigned int b)
		{
			return (b > 1 ? a * Pow(a, b - 1) : a);
		}

		constexpr int Fact(unsigned int n) 
		{
			return n ? (n * Fact(n - 1)) : 1;
		}


		float Fmod(float a, float b);

		float Abs(float a);
		float Sign(float a);
		float IsNeg(float a);

		// based on radians
		float Sin(float a);
		float Cos(float a);
		float Ceil(float a);
	}
}
