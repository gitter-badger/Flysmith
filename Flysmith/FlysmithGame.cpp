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
	m_scene.entities[wing1EntityId].GetTransform()->TranslateY(0.1f);
	m_scene.entities[wing1EntityId].GetTransform()->TranslateX(1.4f);
	auto rcWing1 = m_scene.CreateRenderComponent(m_resources.GetHandle("Wing"), vert, pixel);
	m_scene.entities[wing1EntityId].AttachComponent(rcWing1, &m_scene.renderComponents[rcWing1.index]);

	auto wing2EntityId = m_scene.CreateEntity();
	m_scene.entities[wing2EntityId].GetTransform()->RotateY(-XM_PIDIV2);
	m_scene.entities[wing2EntityId].GetTransform()->TranslateZ(-2.5f);
	m_scene.entities[wing2EntityId].GetTransform()->TranslateY(0.1f);
	m_scene.entities[wing2EntityId].GetTransform()->TranslateX(1.4f);
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

	Wing wing;
	wing.airfoilFile = L"NACA2412";
	
	WingRing root;
	root.chord = 1.6256f;
	root.locationOnWing = 0.0f;
	root.incidenceAngle = 1.0f + 44.0f / 60.0f; // 1 degree 44 minutes
	
	WingRing tip;
	tip.chord = 1.1303f;
	//tip.chord = 1.6256f;
	tip.locationOnWing = 1.0f;
	tip.incidenceAngle = -(1.0f + 30.0f / 60.0f); // - 1 degree 30 minutes

	wing.rings.push_back(root);
	wing.rings.push_back(tip);

	auto wingMesh = wing.GenerateMesh();
	m_resources.AddResource("Wing", m_pRenderer->CacheMesh(wingMesh.verts, wingMesh.indices));

	Fuselage fuselage;
	auto fuselageMesh = fuselage.GenerateMesh(assLocator.GetAssetDirectory(AssetType::FUSELAGES) + L"Cessna172S");
	m_resources.AddResource("Fuselage", m_pRenderer->CacheMesh(fuselageMesh.verts, fuselageMesh.indices));
}

void FlysmithGame::UpdateScene(float dt)
{
	//m_scene.entities[0].GetTransform()->TranslateZ(-dt);
}