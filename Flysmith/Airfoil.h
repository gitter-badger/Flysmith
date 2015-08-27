#pragma once
#include "PCH.h"
#include "Mesh.h"


class Airfoil
{
public:
	Airfoil(const std::wstring& filename);
	Mesh GenerateMesh();

	std::vector<DirectX::XMFLOAT2> points;
};