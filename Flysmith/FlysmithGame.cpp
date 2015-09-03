#include "PCH.h"
#include "FlysmithGame.h"
#include "Airfoil.h"
#include "KeyboardEvents.h"
#include "RenderComponent.h"
#include "Wing.h"
#include "Plane.h"
#include "Entity.h"
using namespace DirectX;


FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
	, m_camController(&m_scene.camTransform)
{
	LoadResources();

	auto wingMesh = m_resources.GetHandle("Wing");
	U32 vert = m_resources.GetHandle("TestVS");
	U32 pixel = m_resources.GetHandle("TestPS");

	RenderComponent comp(m_pRenderer, wingMesh, vert, pixel);
	RenderComponent comp2(m_pRenderer, wingMesh, vert, pixel);
	
	Entity test;
	test.pSceneNode = m_scene.sceneGraph.AddNode();
	test.AttachComponent(&comp);
	m_scene.entities.push_back(test);

	Entity test2;
	test2.pSceneNode = m_scene.sceneGraph.AddNode();
	test2.AttachComponent(&comp2);
	m_scene.entities.push_back(test2);

	m_scene.renderComponents.push_back(comp);
	m_scene.renderComponents.push_back(comp2);
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
	m_scene.entities[0].pSceneNode->transform.RotateY(dt);
}