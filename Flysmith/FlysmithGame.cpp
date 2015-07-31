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
	auto numPoints = foil.points.size();
	auto& points = foil.points;
	for (int i = 1; i < numPoints; i++)
	{
		auto v0 = points[i];
		auto v1 = points[numPoints - i];
		auto v2 = points[i - 1];

		XMStoreFloat2(&v0, XMVector4Transform(XMLoadFloat2(&v0), rot));
		XMStoreFloat2(&v1, XMVector4Transform(XMLoadFloat2(&v1), rot));
		XMStoreFloat2(&v2, XMVector4Transform(XMLoadFloat2(&v2), rot));

		mesh.AddVertex(v0);
		mesh.AddVertex(v1);
		mesh.AddVertex(v2);
	}
	m_pRenderer->SubmitMesh(mesh);
}