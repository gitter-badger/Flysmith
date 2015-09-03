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
	RegisterForEvent("KeyUp"_HASH);

	LoadResources();

	auto planeMesh = m_resources.GetHandle("Plane");
	auto foilMesh = m_resources.GetHandle("Foil");
	auto wingMesh = m_resources.GetHandle("Wing");
	U32 vert = m_resources.GetHandle("TestVS");
	U32 pixel = m_resources.GetHandle("TestPS");

	Entity test;
	m_scene.sceneGraph.nodes.push_back(SceneNode());
	m_scene.sceneGraph.nodes[0].id = 0;
	m_scene.sceneGraph.nodes[0].parent = -1;
	m_scene.sceneGraph.nodes[0].pSceneGraph = &m_scene.sceneGraph;
	test.pSceneNode = &m_scene.sceneGraph.nodes[0];
	
	RenderComponent comp(m_pRenderer, wingMesh, vert, pixel);
	comp.AttachToEntity(test.GetId());
	m_scene.renderComponents.push_back(comp);

	m_scene.entities.push_back(test);

	Entity test2;
	m_scene.sceneGraph.nodes.push_back(SceneNode());
	m_scene.sceneGraph.nodes[1].id = 1;
	m_scene.sceneGraph.nodes[1].parent = 0;
	m_scene.sceneGraph.nodes[1].pSceneGraph = &m_scene.sceneGraph;
	test2.pSceneNode = &m_scene.sceneGraph.nodes[1];

	RenderComponent comp2(m_pRenderer, wingMesh, vert, pixel);
	comp2.AttachToEntity(test2.GetId());
	m_scene.renderComponents.push_back(comp2);

	m_scene.entities.push_back(test2);

	/*RenderComponent comp2(m_pRenderer, foilMesh, vert, pixel);
	m_scene.m_renderComponents.push_back(comp2);

	RenderComponent comp3(m_pRenderer, wingMesh, vert, pixel);
	m_scene.m_renderComponents.push_back(comp3);*/
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