#include "PCH.h"
#include "FlysmithGame.h"
#include "Airfoil.h"
#include "KeyboardEvents.h"
#include "RenderComponent.h"
#include "Wing.h"
#include "Plane.h"
#include "Entity.h"
#include "Fuselage.h"
using namespace DirectX;


FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
	, m_camController(&m_scene.camTransform)
{
	LoadResources();

	auto wingMesh = m_resources.GetHandle("Wing");
	auto planeMesh = m_resources.GetHandle("Plane");
	U32 vert = m_resources.GetHandle("TestVS");
	U32 pixel = m_resources.GetHandle("TestPS");

	/*auto comp0 = m_scene.CreateRenderComponent(wingMesh, vert, pixel),
		 comp1 = m_scene.CreateRenderComponent(wingMesh, vert, pixel),
		 comp2 = m_scene.CreateRenderComponent(wingMesh, vert, pixel);
	
	m_scene.CreateEntity();
	m_scene.entities[0].AttachComponent(comp0, &m_scene.renderComponents[comp0.index]);

	m_scene.CreateEntity(Transform({ 0.0f, 2.0f, 0.0f }), 0);
	m_scene.entities[1].AttachComponent(comp1, &m_scene.renderComponents[comp1.index]);

	m_scene.CreateEntity(Transform({ 0.0f, 4.0f, 0.0f }), 1);
	m_scene.entities[2].AttachComponent(comp2, &m_scene.renderComponents[comp2.index]);*/
	auto renderComp = m_scene.CreateRenderComponent(planeMesh, vert, pixel);
	m_scene.CreateEntity();
	m_scene.entities[0].AttachComponent(renderComp, &m_scene.renderComponents[renderComp.index]);

	auto fuseComp = m_scene.CreateRenderComponent(m_resources.GetHandle("Fuselage"), vert, pixel);
	m_scene.CreateEntity();
	m_scene.entities[1].AttachComponent(fuseComp, &m_scene.renderComponents[fuseComp.index]);
	m_scene.entities[1].GetTransform()->SetRotation(0.0f, XM_PIDIV2, 0.0f);
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

	Fuselage fuselage;
	auto fuselageMesh = fuselage.GenerateMesh();
	m_resources.AddResource("Fuselage", m_pRenderer->CacheMesh(fuselageMesh.verts, fuselageMesh.indices));
}

void FlysmithGame::UpdateScene(float dt)
{
	//m_scene.entities[0].pSceneNode->transform.RotateY(dt);
	//m_scene.entities[1].pSceneNode->transform.TranslateX(dt / 4);
}