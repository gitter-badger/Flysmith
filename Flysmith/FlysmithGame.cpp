#include "PCH.h"
#include "KeyboardEvents.h"
#include "FlysmithGame.h"
#include "Entity.h"

#include "Resources\AssetLocator.h"
#include "Aircraft\Fuselage.h"
#include "Aircraft\Wing.h"
#include "Aircraft\PlaneFactory.h"

#include "Forces.h"
#include "GravityForceGenerator.h"

using namespace DirectX;


// TEMP
ForceRegistry forceRegistry;
GravityForceGenerator gravityGen({ 0.0f, -9.8f, 0.0f });


FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
	, m_camController(&m_scene.camTransform)
{
	RegisterForEvent("LMouseDown"_HASH);
	LoadResources();

	PlaneFactory::Create(m_resources, m_scene);

	forceRegistry.AddForceGenerator(&m_scene.physicsComponents[0], &gravityGen);
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
	wing.ReadFromFile(L"Cessna172S");
	auto wingMesh = wing.GenerateMesh();
	m_resources.AddResource("Wing", m_pRenderer->CacheMesh(wingMesh.verts, wingMesh.indices));

	Fuselage fuselage;
	fuselage.ReadFromFile(L"Cessna172S");
	auto fuselageMesh = fuselage.GenerateMesh();
	m_resources.AddResource("Fuselage", m_pRenderer->CacheMesh(fuselageMesh.verts, fuselageMesh.indices));
}

void FlysmithGame::UpdateScene(float dt)
{
	forceRegistry.UpdateForces(dt);

	// TEMP physics
	for(auto& obj : m_scene.physicsComponents)
	{
		// Apply forces on object
		obj.AddForce({ 0.f, -900.f, 0.f });

		auto xform = m_scene.entities[obj.GetEntityId()].GetTransform();
		obj.Integrate(xform, dt);
	}
}