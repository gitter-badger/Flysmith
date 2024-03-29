#pragma once
#include "PublicDef.h"
#include <DirectXMath.h>


struct KEBABCOMMON_API Vector2
{
	float x;
	float y;
	
	// Set all components to 0
	Vector2();

	// Set the x and y components to the passed @x and @y
	Vector2(float x, float y);

	// Set all components to @uniformValue
	Vector2(float uniformValue);

	DirectX::XMVECTOR GetXMVec();
	void Set(DirectX::CXMVECTOR);
};

struct KEBABCOMMON_API Vector3
{
	float x;
	float y;
	float z;

	// Set all components to 0
	Vector3();

	// Set the x, y and z components to the passed @x, @y and @z
	Vector3(float x, float y, float z);

	// Set all components to @uniformValue
	Vector3(float uniformValue);

	// Set x and y to @vec's x and y and the z component to @z
	Vector3(Vector2 vec, float z = 0.0f);

	Vector3& operator=(const Vector3&);
	Vector3 operator+(const Vector3&) const;
	Vector3 operator-(const Vector3&) const;
	Vector3 operator*(float scale) const;
	Vector3 operator/(float scale) const;
	Vector3 operator+=(const Vector3&);
	Vector3 operator-=(const Vector3&);
	Vector3 operator*=(float scale);
	Vector3 operator/=(float scale);

	DirectX::XMVECTOR GetXMVec();
	void Set(DirectX::CXMVECTOR);
};