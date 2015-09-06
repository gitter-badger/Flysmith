#pragma once
#include "PCH.h"
#include "Airfoil.h"
#include "StandardUnits.h"
using namespace DirectX;


class Wing
{
public:
	Wing(const std::wstring& airfoilFile);
	void GenerateMesh();
	Mesh GetMesh();

private:
	Airfoil m_airfoil;
	Mesh m_mesh;
};