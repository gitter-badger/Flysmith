#include "PCH.h"
#include "KeyboardEvents.h"
#include "FlysmithGame.h"
#include "Entity.h"

#include "Resources\AssetLocator.h"
#include "Aircraft\Fuselage.h"
#include "Aircraft\Wing.h"
#include "Aircraft\PlaneFactory.h"
using namespace DirectX;


FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
	, m_camController(&m_scene.camTransform)
{
	RegisterForEvent("LMouseDown"_HASH);
	LoadResources();

	PlaneFactory::Create(m_resources, m_scene);
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
	// TEMP physics
	for(auto& obj : m_scene.physicsComponents)
	{
		// Ignore things with infinite mass.
		if (obj.inverseMass <= 0.0f) continue;
	
		auto vel = obj.velocity.GetXMVec();
		auto acc = obj.acceleration.GetXMVec();
		
		auto xform = m_scene.entities[obj.GetEntityId()].GetTransform();
		auto pos = xform->GetPositionXM();
		xform->SetPosition(pos + vel * dt);

		obj.velocity.Set(vel + dt * acc);
		
		vel = obj.velocity.GetXMVec();
		obj.velocity.Set(vel * powf(dt, obj.damping));
	}
}