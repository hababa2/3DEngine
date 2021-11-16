#pragma once

namespace nh
{
	const float Pi = 3.14159265359f;		//180
	const float TwoPi = 6.28318530718f;		//360
	const float HalfPi = 1.57079632679f;	//90
	const float QuarterPi = 0.78539816339f;	//45

	const float pConst = 57.2957795131f;
	const float pConst2 = 0.01745329251f;

	inline float RadToDeg(float radians)
	{
		return radians * pConst;
	}

	inline float DegToRad(float degrees)
	{
		return degrees * pConst2;
	}

	template<typename T>
	inline T Max(T v1, T v2)
	{
		return v1 > v2 ? v1 : v2;
	}

	template<typename T>
	inline T Min(T v1, T v2)
	{
		return v1 < v2 ? v1 : v2;
	}

	template<typename T>
	inline T Abs(T v)
	{
		return v > 0 ? v : -v;
	}

	template<typename T>
	inline T Clamp(T v, T min, T max)
	{
		return v < min ? min : v > max ? max : v;
	}

	template<typename T>
	inline T Wrap(T v, T min, T max)
	{
		return v < min ? max : v > max ? min : v;
	}

	template<typename T>
	inline T Lerp(T a, T b, float t)
	{
		return a + ((b - a) * Clamp(t, 0.0f, 1.0f));
	}
}