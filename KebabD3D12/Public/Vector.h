#pragma once
#include "PublicDef.h"
#include <DirectXMath.h>


struct KEBAB12_API Vector2
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

struct KEBAB12_API Vector3
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

	DirectX::XMVECTOR GetXMVec();
	void Set(DirectX::CXMVECTOR);
};