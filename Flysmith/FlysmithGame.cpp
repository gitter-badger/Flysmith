#include "PCH.h"
#include "RenderComponent.h"
#include "KeyboardEvents.h"
#include "FlysmithGame.h"
#include "Math\AngleMath.h"
#include "Entity.h"

#include "Resources\AssetLocator.h"
#include "Aircraft\Fuselage.h"
#include "Aircraft\Wing.h"
using namespace DirectX;


FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
	, m_camController(&m_scene.camTransform)
{
	RegisterForEvent("LMouseDown"_HASH);
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
	auto rightWingEntityId = m_scene.CreateEntity();
	m_scene.entities[rightWingEntityId].GetTransform()->RotateY(-XM_PIDIV2);
	auto rcWing1 = m_scene.CreateRenderComponent(m_resources.GetHandle("Wing"), vert, pixel);
	m_scene.entities[rightWingEntityId].AttachComponent(rcWing1, &m_scene.renderComponents[rcWing1.index]);

	auto leftWingEntityId = m_scene.CreateEntity();
	m_scene.entities[leftWingEntityId].GetTransform()->MirrorAlongX();
	m_scene.entities[leftWingEntityId].GetTransform()->RotateY(XM_PIDIV2);
	auto rcWing2 = m_scene.CreateRenderComponent(m_resources.GetHandle("Wing"), vert, pixel);
	m_scene.entities[leftWingEntityId].AttachComponent(rcWing2, &m_scene.renderComponents[rcWing2.index]);

	// Attach
	m_scene.entities[planeEntityId].AddChild(&m_scene.entities[fuselageEntityId]);
	m_scene.entities[planeEntityId].AddChild(&m_scene.entities[rightWingEntityId]);
	m_scene.entities[planeEntityId].AddChild(&m_scene.entities[leftWingEntityId]);
}

Wing wing;
bool enabled = false;

void FlysmithGame::HandleEvent(const Event& ev)
{
	switch (ev.type)
	{
	case "LMouseDown"_HASH:
		enabled = true;
	}
}

void FlysmithGame::LoadResources()
{
	AssetLocator assLocator;
	m_resources.AddResource("TestVS", m_pRenderer->CacheShader(VERTEX_SHADER, assLocator.GetAssetDirectory(AssetType::SHADERS) + L"TestVS.hlsl"));
	m_resources.AddResource("TestPS", m_pRenderer->CacheShader(PIXEL_SHADER, assLocator.GetAssetDirectory(AssetType::SHADERS) + L"TestPS.hlsl"));

	wing.ReadFromFile(L"Cessna172S");

	auto wingMesh = wing.GenerateMesh();
	m_resources.AddResource("Wing", m_pRenderer->CacheMesh(wingMesh.verts, wingMesh.indices));

	Fuselage fuselage;
	auto fuselageMesh = fuselage.GenerateMesh(assLocator.GetAssetDirectory(AssetType::FUSELAGES) + L"Cessna172S");
	m_resources.AddResource("Fuselage", m_pRenderer->CacheMesh(fuselageMesh.verts, fuselageMesh.indices));
}

float dicks = 0.0f;

void FlysmithGame::UpdateScene(float dt)
{
	if (enabled)
	{
		dicks += dt;
		if (dicks >= 1.f)
		{
			dicks = 0.0f;
			wing.sections[0].sweep = 1.0f;
			auto wingMesh = wing.GenerateMesh();
			m_pRenderer->UpdateMesh(0, wingMesh.verts, wingMesh.indices);
		}
	}
}