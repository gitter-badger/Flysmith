#include "PCH.h"
#include "FlysmithGame.h"
#include "EventListener.h"
#include "Event.h"
#include "InputEvents.h"
using namespace cuc;
#include "../KebabD3D12/Public/Mesh.h"
#include "Airfoil.h"
using namespace DirectX;


FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
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
}