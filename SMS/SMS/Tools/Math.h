///
/// \Brief		The Math.h header defines various mathematical functions and several macros.
/// 

#pragma once

///
/// \brief		Predefined mathematical constants.
///

///
/// \brief		Pi, the ratio of a circle’s circumference to its diameter.
///
#define SMS_PI_F	3.1415927f

///
/// \brief		Pi divided by two.
///
#define SMS_PI_2_F	1.5707963f

///
/// \brief		Pi divided by four.
///
#define SMS_PI_4_F	0.7853982f

///
/// \brief		Functions to compute common mathematical operations and transformations.
///

///
/// \brief		Computes the power of a number.
///
#define	POW2(x)		(x * x)
#define POW3(x)		(x * x * x)
#define POW5(x)		(x * x * x * x * x)
#define POW7(x)		(x * x * x * x * x * x * x)

///
/// \brief		Returns the remainder of x divided by y
///
float fmod(float a, float b);

///
/// \brief		Factorial of a positive integer n (denoted by n!)
///
/// \details	Product of all positive integers less than or equal to n
///				The factorial of a positive number n is given by : n! = 1 * 2 * 3 * 4....n
///				The factorial of a negative number doesn't exist.
///				Factorial of 0 is 1. And 0! = 1
constexpr int fact(int n);

///
/// \brief		Returns sine of approximate of a radian angle x.
///
float MySin(float a);

// END
