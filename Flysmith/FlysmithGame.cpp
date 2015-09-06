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

	auto renderComp = m_scene.CreateRenderComponent(m_resources.GetHandle("Plane"), vert, pixel);
	m_scene.CreateEntity();
	m_scene.entities[0].AttachComponent(renderComp, &m_scene.renderComponents[renderComp.index]);

	auto fuseComp = m_scene.CreateRenderComponent(m_resources.GetHandle("Fuselage"), vert, pixel);
	m_scene.CreateEntity();
	m_scene.entities[1].AttachComponent(fuseComp, &m_scene.renderComponents[fuseComp.index]);
	m_scene.entities[1].GetTransform()->SetRotation(0.0f, XM_PIDIV2, 0.0f);

	Atmosphere atmo;
	g_logger.Write(atmo.GetTemperature(0.0f));
	g_logger.Write(atmo.GetTemperature(3451.0f));
	g_logger.Write(atmo.GetTemperature(15000.0f));
	g_logger.Write(atmo.GetTemperature(17000.0f));
}

void FlysmithGame::HandleEvent(const Event& ev)
{
}

void FlysmithGame::LoadResources()
{
	AssetLocator assLocator;
	m_resources.AddResource("TestVS", m_pRenderer->CacheShader(VERTEX_SHADER, assLocator.GetAssetDirectory(AssetType::SHADERS) + L"TestVS.hlsl"));
	m_resources.AddResource("TestPS", m_pRenderer->CacheShader(PIXEL_SHADER, assLocator.GetAssetDirectory(AssetType::SHADERS) + L"TestPS.hlsl"));

	Airfoil foil(L"NACA4415.dat");
	auto foilMesh = foil.GenerateMesh();
	m_resources.AddResource("Foil", m_pRenderer->CacheMesh(foilMesh.verts, foilMesh.indices));

	Wing wing(L"NACA4415.dat");
	auto wingMesh = wing.GetMesh();
	m_resources.AddResource("Wing", m_pRenderer->CacheMesh(wingMesh.verts, wingMesh.indices));

	Plane plane;
	plane.AddWingPair(wing, MetersToDXUnits(-1.5f), MetersToDXUnits(0.5f), MetersToDXUnits(0.0f));
	auto planeMesh = plane.GetMesh();
	m_resources.AddResource("Plane", m_pRenderer->CacheMesh(planeMesh.verts, planeMesh.indices));

	Fuselage fuselage;
	auto fuselageMesh = fuselage.GenerateMesh();
	m_resources.AddResource("Fuselage", m_pRenderer->CacheMesh(fuselageMesh.verts, fuselageMesh.indices));
}

void FlysmithGame::UpdateScene(float dt)
{
}