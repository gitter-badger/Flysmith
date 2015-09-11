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
	auto rightWingEntityId = m_scene.CreateEntity();
	m_scene.entities[rightWingEntityId].GetTransform()->RotateY(-XM_PIDIV2);
	/*m_scene.entities[rightWingEntityId].GetTransform()->TranslateY(0.1f);
	m_scene.entities[rightWingEntityId].GetTransform()->TranslateX(1.4f);*/
	auto rcWing1 = m_scene.CreateRenderComponent(m_resources.GetHandle("Wing"), vert, pixel);
	m_scene.entities[rightWingEntityId].AttachComponent(rcWing1, &m_scene.renderComponents[rcWing1.index]);

	auto leftWingEntityId = m_scene.CreateEntity();
	// Mirror along X
	m_scene.entities[leftWingEntityId].GetTransform()->MirrorAlongX();
	m_scene.entities[leftWingEntityId].GetTransform()->RotateY(XM_PIDIV2);
	//m_scene.entities[leftWingEntityId].GetTransform()->TranslateY(1);
	/*m_scene.entities[leftWingEntityId].GetTransform()->TranslateY(1.5f);
	m_scene.entities[leftWingEntityId].GetTransform()->TranslateX(1.4f);*/
	auto rcWing2 = m_scene.CreateRenderComponent(m_resources.GetHandle("Wing"), vert, pixel);
	m_scene.entities[leftWingEntityId].AttachComponent(rcWing2, &m_scene.renderComponents[rcWing2.index]);

	// Attach
	m_scene.entities[planeEntityId].AddChild(&m_scene.entities[fuselageEntityId]);
	m_scene.entities[planeEntityId].AddChild(&m_scene.entities[rightWingEntityId]);
	m_scene.entities[planeEntityId].AddChild(&m_scene.entities[leftWingEntityId]);
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
	wing.length = 5.0f;
	
	WingRing root;
	root.chord = 1.6256f;
	root.locationOnWing = 0.0f;
	root.incidenceAngle = 1.0f + 30.0f / 60.0f; // 1 degree 30 minutes
	
	WingRing tip;
	tip.chord = 1.1303f;
	tip.locationOnWing = 1.0f;
	tip.incidenceAngle = -(1.0f + 30.0f / 60.0f); // - 1 degree 30 minutes

	wing.rings.push_back(root);
	wing.rings.push_back(tip);

	wing.sections.push_back(WingSection());
	wing.sections[0].sweep = 0.0f;
	wing.sections[0].dihedral = 1.0f + 44.0f / 60.0f; // 1 degree 44 minutes dihedral

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