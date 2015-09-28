#pragma once


constexpr float PI = 3.141592654f;

// Converts angle measurement in degrees/minutes/seconds format to degrees in decimal format
inline float DegMinSecToDecimal(float degrees, float minutes = 0.0f, float seconds = 0.0f)
{
	auto sign = degrees < 0.0f ? -1 : 1;
	return degrees + 
		   sign * minutes / 60.0f + 
		   sign * seconds / 3600.0f;
}

inline float DegToRad(float degrees) 
{ 
	static constexpr float conversionFactor = PI / 180.0f;
	return degrees * conversionFactor;
}

inline float RadToDeg(float radians) 
{ 
	static constexpr float conversionFactor = 180.0f / PI;
	return radians * conversionFactor;
}