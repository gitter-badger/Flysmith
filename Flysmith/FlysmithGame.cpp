#include "PCH.h"
#include "FlysmithGame.h"
#include "Airfoil.h"
#include "KeyboardEvents.h"
using namespace cuc;
using namespace DirectX;


std::wstring foils[] = {
	L"S3021.dat",
	L"S1020.dat",
	L"NLF1015.dat",
	L"NACA4415.dat",
	L"ah94w301.dat", 
	L"davissm.dat"
};
U32 foilIdx = 0;
U32 numFoils = 6;

FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
	, m_camController(&m_scene.m_camTransform)
{
	RegisterForEvent("KeyUp"_HASH);

	GenerateFoilMesh(foils[foilIdx]);
	m_scene.objTransforms.push_back(Transform());
}

void FlysmithGame::HandleEvent(const Event& ev)
{
	switch (ev.type)
	{
	case "KeyUp"_HASH:
		if (ev.data[0].asU32 == 'B')
		{
			foilIdx = (foilIdx + 1) % numFoils;
			GenerateFoilMesh(foils[foilIdx]);
		}
		break;
	}
}

void FlysmithGame::GenerateFoilMesh(const std::wstring& foilName)
{
	Airfoil foil;
	foil.LoadFromFile(foilName.c_str());

	std::vector<Vertex> verts;
	std::vector<U32> indices;
	for (auto& foilPoint : foil.points)
	{
		XMFLOAT2 vert(foilPoint);
		vert.x -= .5f;
		vert.y -= .5f;
		verts.push_back({ { vert.x, vert.y, 0.0f }, {0.0f, 0.0f, 0.0f, 1.0f} });
	}

	auto numPoints = foil.points.size();
	for (U32 i = 1; i < numPoints; i++)
	{
		indices.push_back(i);
		indices.push_back(numPoints - i);
		indices.push_back(i - 1);
	}

	m_pRenderer->CacheMesh(verts, indices);
}

void FlysmithGame::UpdateScene(float dt)
{
	// Do stuff with tempObjTransform
	m_scene.objTransforms[0].RotateY(1.0f * dt);
	// Do stuff with tempCamTransform
}