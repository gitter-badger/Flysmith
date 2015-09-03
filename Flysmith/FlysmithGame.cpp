#include "PCH.h"
#include "FlysmithGame.h"
#include "Airfoil.h"
#include "KeyboardEvents.h"
#include "../KebabD3D12/Public/RenderComponent.h"
#include "Wing.h"
#include "Plane.h"
#include "Entity.h"
using namespace DirectX;


FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
	, m_camController(&m_scene.m_camTransform)
{
	RegisterForEvent("KeyUp"_HASH);

	LoadResources();

	auto planeMesh = m_resources.GetHandle("Plane");
	auto foilMesh = m_resources.GetHandle("Foil");
	auto wingMesh = m_resources.GetHandle("Wing");
	U32 vert = m_resources.GetHandle("TestVS");
	U32 pixel = m_resources.GetHandle("TestPS");

	RenderComponent comp(m_pRenderer, planeMesh, vert, pixel);
	m_scene.m_renderComponents.push_back(comp);

	Entity test;
	comp.AttachToEntity(test.GetId());

	/*RenderComponent comp2(m_pRenderer, foilMesh, vert, pixel);
	m_scene.m_renderComponents.push_back(comp2);

	RenderComponent comp3(m_pRenderer, wingMesh, vert, pixel);
	m_scene.m_renderComponents.push_back(comp3);*/
}

void FlysmithGame::HandleEvent(const Event& ev)
{
	switch (ev.type)
	{
	}
}

void FlysmithGame::LoadResources()
{
	m_resources.AddResource("TestVS", m_pRenderer->CacheShader(VERTEX_SHADER, L"E:\\Flysmith\\KebabD3D12\\Private\\Shaders\\TestVS.hlsl"));
	m_resources.AddResource("TestPS", m_pRenderer->CacheShader(PIXEL_SHADER, L"E:\\Flysmith\\KebabD3D12\\Private\\Shaders\\TestPS.hlsl"));

	Airfoil foil(L"NACA4415.dat");
	auto foilMesh = foil.GenerateMesh();
	m_resources.AddResource("Foil", m_pRenderer->CacheMesh(foilMesh.verts, foilMesh.indices));

	Wing wing(L"NACA4415.dat");
	auto wingMesh = wing.GetMesh();
	m_resources.AddResource("Wing", m_pRenderer->CacheMesh(wingMesh.verts, wingMesh.indices));

	Plane plane;
	plane.AddWingPair(wing, MetersToDXUnits(0), MetersToDXUnits(0), MetersToDXUnits(2));
	plane.AddWingPair(wing, MetersToDXUnits(0), MetersToDXUnits(1.5f), MetersToDXUnits(2));
	auto planeMesh = plane.GetMesh();
	m_resources.AddResource("Plane", m_pRenderer->CacheMesh(planeMesh.verts, planeMesh.indices));
}

void FlysmithGame::UpdateScene(float dt)
{
	//m_scene.m_renderComponents[0].m_transform.SetPosition(0.0f, 0.0, 0.0f);
	//m_scene.m_renderComponents[0].m_transform.SetRotation(0.0f, XM_PIDIV2, 0.0f);
}