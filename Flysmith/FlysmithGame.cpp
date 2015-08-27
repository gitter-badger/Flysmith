#include "PCH.h"
#include "FlysmithGame.h"
#include "Airfoil.h"
#include "KeyboardEvents.h"
#include "../KebabD3D12/Public/RenderComponent.h"
using namespace DirectX;


FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
	, m_camController(&m_scene.m_camTransform)
{
	RegisterForEvent("KeyUp"_HASH);

	LoadResources();

	U32 mesh = m_resources.GetHandle("NACA2412");
	U32 vert = m_resources.GetHandle("TestVS");
	U32 pixel = m_resources.GetHandle("TestPS");
	RenderComponent fuckyoufuckyoufuckyou(m_pRenderer, mesh, vert, pixel);
	m_scene.m_renderComponents.push_back(fuckyoufuckyoufuckyou);
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
}

void FlysmithGame::UpdateScene(float dt)
{
	// Do stuff with tempObjTransform
	m_scene.m_renderComponents[0].m_transform.RotateY(1.0f * dt);
	// Do stuff with tempCamTransform
}