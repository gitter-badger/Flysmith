#include "PCH.h"
#include "KeyboardEvents.h"
#include "FlysmithGame.h"
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

	auto vert = m_resources.GetHandle("TestVS");
	auto pixel = m_resources.GetHandle("TestPS");

	// Plane
	auto planeEntityId = m_scene.CreateEntity();

	// Fuselage
	auto fuselageEntityId = m_scene.CreateEntity();
	auto rcFuselage = m_scene.CreateRenderComponent(m_resources.GetHandle("Fuselage"), vert, pixel);
	m_scene.AttachComponent(fuselageEntityId, rcFuselage);
	
	// Wings
	auto rightWingEntityId = m_scene.CreateEntity();
	m_scene.entities[rightWingEntityId].GetTransform()->RotateY(-XM_PIDIV2);
	auto rcWing1 = m_scene.CreateRenderComponent(m_resources.GetHandle("Wing"), vert, pixel);
	m_scene.AttachComponent(rightWingEntityId, rcWing1);
	
	auto leftWingEntityId = m_scene.CreateEntity();
	m_scene.entities[leftWingEntityId].GetTransform()->MirrorAlongX();
	m_scene.entities[leftWingEntityId].GetTransform()->RotateY(XM_PIDIV2);
	auto rcWing2 = m_scene.CreateRenderComponent(m_resources.GetHandle("Wing"), vert, pixel);
	m_scene.AttachComponent(leftWingEntityId, rcWing2);
	
	// Attach
	m_scene.entities[planeEntityId].AddChild(&m_scene.entities[fuselageEntityId]);
	m_scene.entities[planeEntityId].AddChild(&m_scene.entities[rightWingEntityId]);
	m_scene.entities[planeEntityId].AddChild(&m_scene.entities[leftWingEntityId]);
}

bool enabled = false;

void FlysmithGame::HandleEvent(const Event& ev)
{
	switch (ev.type)
	{
	case "LMouseDown"_HASH:
		OutputDebugStringA("CLICK\n");
		enabled = true;
	}
}

void FlysmithGame::LoadResources()
{
	AssetLocator assLocator;
	m_resources.AddResource("TestVS", m_pRenderer->CacheShader(VERTEX_SHADER, assLocator.GetAssetDirectory(AssetType::SHADERS) + L"TestVS.hlsl"));
	m_resources.AddResource("TestPS", m_pRenderer->CacheShader(PIXEL_SHADER, assLocator.GetAssetDirectory(AssetType::SHADERS) + L"TestPS.hlsl"));

	Wing wing;
	wing.ReadFromFile(L"Cessna172S");
	auto wingMesh = wing.GenerateMesh();
	m_resources.AddResource("Wing", m_pRenderer->CacheMesh(wingMesh.verts, wingMesh.indices));

	Fuselage fuselage;
	fuselage.ReadFromFile(L"Cessna172S");
	auto fuselageMesh = fuselage.GenerateMesh();
	m_resources.AddResource("Fuselage", m_pRenderer->CacheMesh(fuselageMesh.verts, fuselageMesh.indices));
}

float dicks = 0.0f;
float sweep = 0.0f;

void FlysmithGame::UpdateScene(float dt)
{
	if (enabled)
	{
		dicks += dt;
		if (dicks >= 1.f)
		{
			dicks = 0.0f;
			sweep++;

			Wing wing;
			wing.ReadFromFile(L"Cessna172S");
			wing.sections[0].sweep = sweep;
			auto wingMesh = wing.GenerateMesh();
			m_pRenderer->UpdateMesh(0, wingMesh.verts, wingMesh.indices);
		}
	}
}