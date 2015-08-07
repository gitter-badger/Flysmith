#include "PCH.h"
#include "FlysmithGame.h"
#include "Airfoil.h"
#include "../KebabD3D12/Public/Mesh.h"
#include "InputEvents.h"
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
	RegisterForEvent(KeyboardEvent::KeyUpId);

	GenerateFoilMesh(foils[foilIdx]);
	m_scene.objTransforms.push_back(Transform());
}

void FlysmithGame::HandleEvent(const Event& ev)
{
	switch (ev.type)
	{
	case KeyboardEvent::KeyUpId:
		if (ev[0].GetUnsignedInt() == 'B')
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

	Mesh mesh;
	for (auto& foilPoint : foil.points)
	{
		XMFLOAT2 vert(foilPoint);
		vert.x -= .5f;
		vert.y -= .5f;
		mesh.AddVertex(vert);
	}

	auto numPoints = foil.points.size();
	for (U32 i = 1; i < numPoints; i++)
		mesh.SetTriangle(i, numPoints - i, i - 1);

	m_pRenderer->CacheMesh(mesh);
}

void FlysmithGame::UpdateScene(float dt)
{
	// Do stuff with tempObjTransform
	m_scene.objTransforms[0].RotateY(1.0f * dt);
	// Do stuff with tempCamTransform
}