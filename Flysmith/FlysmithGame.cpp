#include "PCH.h"
#include "FlysmithGame.h"
#include "Airfoil.h"
#include "KeyboardEvents.h"
#include "RenderComponent.h"
#include "Wing.h"
#include "Plane.h"
#include "Entity.h"
#include "Fuselage.h"
#include "Atmosphere.h"
#include "AssetLocator.h"
using namespace DirectX;


FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
	, m_camController(&m_scene.camTransform)
{
	LoadResources();

	U32 vert = m_resources.GetHandle("TestVS");
	U32 pixel = m_resources.GetHandle("TestPS");

	// Plane
	auto planeEntityId = m_scene.CreateEntity();

	// Fuselage
	auto fuselageEntityId = m_scene.CreateEntity();
	auto rcFuselage = m_scene.CreateRenderComponent(m_resources.GetHandle("Fuselage"), vert, pixel);
	m_scene.entities[fuselageEntityId].AttachComponent(rcFuselage, &m_scene.renderComponents[rcFuselage.index]);

	// Wings
	auto wing1EntityId = m_scene.CreateEntity();
	m_scene.entities[wing1EntityId].GetTransform()->RotateY(-XM_PIDIV2);
	auto rcWing1 = m_scene.CreateRenderComponent(m_resources.GetHandle("Wing"), vert, pixel);
	m_scene.entities[wing1EntityId].AttachComponent(rcWing1, &m_scene.renderComponents[rcWing1.index]);

	auto wing2EntityId = m_scene.CreateEntity();
	m_scene.entities[wing2EntityId].GetTransform()->RotateY(XM_PIDIV2);
	auto rcWing2 = m_scene.CreateRenderComponent(m_resources.GetHandle("Wing"), vert, pixel);
	m_scene.entities[wing2EntityId].AttachComponent(rcWing2, &m_scene.renderComponents[rcWing2.index]);

	// Attach
	m_scene.entities[planeEntityId].AddChild(&m_scene.entities[fuselageEntityId]);
	m_scene.entities[planeEntityId].AddChild(&m_scene.entities[wing1EntityId]);
	m_scene.entities[planeEntityId].AddChild(&m_scene.entities[wing2EntityId]);
}

void FlysmithGame::HandleEvent(const Event& ev)
{
	switch (ev.type)
	{
	}
}

void FlysmithGame::LoadResources()
{
	AssetLocator assLocator;
	m_resources.AddResource("TestVS", m_pRenderer->CacheShader(VERTEX_SHADER, assLocator.GetAssetDirectory(AssetType::SHADERS) + L"TestVS.hlsl"));
	m_resources.AddResource("TestPS", m_pRenderer->CacheShader(PIXEL_SHADER, assLocator.GetAssetDirectory(AssetType::SHADERS) + L"TestPS.hlsl"));

	Wing wing(L"NACA4415.dat");
	auto wingMesh = wing.GetMesh();
	m_resources.AddResource("Wing", m_pRenderer->CacheMesh(wingMesh.verts, wingMesh.indices));

	Fuselage fuselage;
	auto fuselageMesh = fuselage.GenerateMesh();
	m_resources.AddResource("Fuselage", m_pRenderer->CacheMesh(fuselageMesh.verts, fuselageMesh.indices));
}

void FlysmithGame::UpdateScene(float dt)
{
	m_scene.entities[0].GetTransform()->TranslateZ(-dt);
}