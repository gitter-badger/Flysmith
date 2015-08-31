#include "PCH.h"
#include "FlysmithGame.h"
#include "Airfoil.h"
#include "KeyboardEvents.h"
#include "../KebabD3D12/Public/RenderComponent.h"
#include "Wing.h"
using namespace DirectX;


FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
	, m_camController(&m_scene.m_camTransform)
{
	RegisterForEvent("KeyUp"_HASH);

	LoadResources();

	U32 mesh = m_resources.GetHandle("NACA2412");
	U32 mesh2 = m_resources.GetHandle("NACA2412Wing");
	U32 vert = m_resources.GetHandle("TestVS");
	U32 pixel = m_resources.GetHandle("TestPS");

	RenderComponent comp1(m_pRenderer, mesh, vert, pixel);
	RenderComponent comp2(m_pRenderer, mesh2, vert, pixel);
	m_scene.m_renderComponents.push_back(comp1);
	m_scene.m_renderComponents.push_back(comp2);
}

void FlysmithGame::HandleEvent(const Event& ev)
{
	switch (ev.type)
	{
	}
}

void FlysmithGame::LoadResources()
{
	m_resources.AddResource("TestVS", m_pRenderer->CacheShader(VERTEX_SHADER, L"D:\\Flysmith\\KebabD3D12\\Private\\Shaders\\TestVS.hlsl"));
	m_resources.AddResource("TestPS", m_pRenderer->CacheShader(PIXEL_SHADER, L"D:\\Flysmith\\KebabD3D12\\Private\\Shaders\\TestPS.hlsl"));

	Airfoil airfoil(L"NACA2412.dat");
	auto mesh = airfoil.GenerateMesh();
	m_resources.AddResource("NACA2412", m_pRenderer->CacheMesh(mesh.verts, mesh.indices));

	Wing wing(L"NACA2412.dat");
	auto mesh3 = wing.GenerateMesh();
	m_resources.AddResource("NACA2412Wing", m_pRenderer->CacheMesh(mesh3.verts, mesh3.indices));

	Airfoil airfoil2(L"NACA4415.dat");
	auto mesh2 = airfoil2.GenerateMesh();
	m_resources.AddResource("NACA4415", m_pRenderer->CacheMesh(mesh2.verts, mesh2.indices));
}

void FlysmithGame::UpdateScene(float dt)
{
	// Do stuff with tempObjTransform
	m_scene.m_renderComponents[0].m_transform.SetPosition(0.0f, .5f, 0.0f);
	m_scene.m_renderComponents[1].m_transform.SetPosition(0.0f, 0.0, 0.0f);
	m_scene.m_renderComponents[1].m_transform.SetRotation(0.0f, XM_PIDIV2, 0.0f);
	// Do stuff with tempCamTransform
}