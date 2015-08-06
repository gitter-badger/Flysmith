#include "PCH.h"
#include "FlysmithGame.h"
#include "Airfoil.h"
#include "../KebabD3D12/Public/Mesh.h"
using namespace cuc;
using namespace DirectX;


FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
	, m_camController(&m_scene.m_camTransform)
{
	Airfoil foil;
	foil.LoadFromFile(L"NACA4415.dat");
	
	Mesh mesh;
	auto rotate = XMMatrixRotationZ(-XM_PIDIV2);

	for (auto& foilPoint : foil.points)
	{
		XMFLOAT2 vert(foilPoint);
		XMStoreFloat2(&vert, XMVector4Transform(XMLoadFloat2(&vert), rotate));

		mesh.AddVertex(vert);
	}

	auto numPoints = foil.points.size();
	for (U32 i = 1; i < numPoints; i++)
		mesh.SetTriangle(i, numPoints - i, i - 1);
	
	m_pRenderer->SubmitMesh(mesh);

	m_scene.objTransforms.push_back(Transform());
}

void FlysmithGame::UpdateScene()
{
	// Do stuff with tempObjTransform
	//m_scene.objTransforms[0].TranslateX(.0001f);
	// Do stuff with tempCamTransform
}