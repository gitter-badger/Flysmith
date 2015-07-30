#pragma once
#include "PCH.h"


class Airfoil
{
public:
	bool LoadFromFile(const wchar_t* filename);

	std::vector<DirectX::XMFLOAT2> points;
};