#include "PCH.h"
#include "FlysmithGame.h"
#include "EventListener.h"
#include "Event.h"
#include "InputEvents.h"
using namespace cuc;
#include "../KebabD3D12/Public/Mesh.h"
#include "Airfoil.h"


FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
{
	Airfoil foil;
	foil.LoadFromFile(L"NACA4415");

	Mesh mesh;
	for (int idx = 1; idx < foil.points.size(); idx++)
	{
		auto v0 = foil.points[idx];
		DirectX::XMFLOAT2 v1 = { 0.5f, 0.0f };
		auto v2 = foil.points[idx - 1];

		mesh.AddVertex(v0);
		mesh.AddVertex(v1);
		mesh.AddVertex(v2);
	}
	m_pRenderer->SubmitMesh(mesh);
}