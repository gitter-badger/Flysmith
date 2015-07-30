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

	auto rot = XMMatrixRotationZ(-XM_PIDIV2);

	Mesh mesh;
	for (int idx = 1; idx < foil.points.size(); idx++)
	{
		auto v0 = foil.points[idx];
		XMFLOAT2 v1 = { 0.0f, 0.0f };
		auto v2 = foil.points[idx - 1];

		/*if (v0.x < 0.4f)
			v1.y = 0.2f;
		else if (v0.x < 0.6f)
			v1.y = 0.5f;
		else
			v1.y = 0.8f;*/
		
		XMVECTOR rV0 = { v0.x, v0.y, 0.0f, 1.0f };
		XMVECTOR rV1 = { v1.x, v1.y, 0.0f, 1.0f };
		XMVECTOR rV2 = { v2.x, v2.y, 0.0f, 1.0f };

		rV0 = XMVector4Transform(rV0, rot);
		rV1 = XMVector4Transform(rV1, rot);
		rV2 = XMVector4Transform(rV2, rot);

		XMStoreFloat2(&v0, rV0);
		XMStoreFloat2(&v1, rV1);
		XMStoreFloat2(&v2, rV2);

		mesh.AddVertex(v0);
		mesh.AddVertex(v1);
		mesh.AddVertex(v2);
	}
	m_pRenderer->SubmitMesh(mesh);
}