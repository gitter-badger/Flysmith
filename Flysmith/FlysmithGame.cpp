#include "PCH.h"
#include "KeyboardEvents.h"
#include "FlysmithGame.h"
#include "Entity.h"

#include "Resources\AssetLocator.h"
#include "Aircraft\Fuselage.h"
#include "Aircraft\Wing.h"
using namespace DirectX;


// TEMP for readability 
// Will move into a factory alltogether(and don't do this kind of silly code duplication)
void CreateFuselage(Scene& scene, ResourceRegistry& resources, EntityId planeId)
{
	// Create dummy entity
	auto fuselageId = scene.CreateEntity();

	// Crate render component
	auto renderCompProxy = scene.CreateRenderComponent(resources.GetHandle("Fuselage"),
													   resources.GetHandle("TestVS"),
													   resources.GetHandle("TestPS"));
	scene.AttachComponent(fuselageId, renderCompProxy);

	// Create physics component
	PhysicsComponent physComponent;
	physComponent.inverseMass = 0.1f;
	physComponent.velocity = 0.0f;
	physComponent.acceleration = { 0.0f, -9.8f, 0.0f };
	physComponent.damping = 1.0f;
	auto physComponentProxy = scene.AddComponent(&physComponent);
	scene.AttachComponent(fuselageId, physComponentProxy);

	// Attach the fuselage entity to the dummy master plane entity 
	scene.entities[planeId].AddChild(&scene.entities[fuselageId]);
}

void CreateLeftWing(Scene& scene, ResourceRegistry& resources, EntityId planeId)
{
	// Create dummy entity
	auto wingId = scene.CreateEntity();

	// Set initial transform(relative to the master plane dummy)
	scene.entities[wingId].GetTransform()->MirrorAlongX();
	scene.entities[wingId].GetTransform()->RotateY(XM_PIDIV2);

	// Crate render component
	auto renderCompProxy = scene.CreateRenderComponent(resources.GetHandle("Wing"),
													   resources.GetHandle("TestVS"),
													   resources.GetHandle("TestPS"));
	scene.AttachComponent(wingId, renderCompProxy);

	// Create physics component


	// Attach the fuselage entity to the dummy master plane entity 
	scene.entities[planeId].AddChild(&scene.entities[wingId]);
}

void CreateRightWing(Scene& scene, ResourceRegistry& resources, EntityId planeId)
{
	// Create dummy entity
	auto wingId = scene.CreateEntity();

	// Set initial transform(relative to the master plane dummy)
	scene.entities[wingId].GetTransform()->RotateY(-XM_PIDIV2);

	// Crate render component
	auto renderCompProxy = scene.CreateRenderComponent(resources.GetHandle("Wing"),
													   resources.GetHandle("TestVS"),
													   resources.GetHandle("TestPS"));
	scene.AttachComponent(wingId, renderCompProxy);

	// Create physics component


	// Attach the fuselage entity to the dummy master plane entity 
	scene.entities[planeId].AddChild(&scene.entities[wingId]);
}

FlysmithGame::FlysmithGame(HINSTANCE hInstance)
	: Application(hInstance)
	, m_camController(&m_scene.camTransform)
{
	RegisterForEvent("LMouseDown"_HASH);
	LoadResources();

	// TEMP for readability 
	auto planeEntityId = m_scene.CreateEntity();
	CreateFuselage(m_scene, m_resources, planeEntityId);
	CreateLeftWing(m_scene, m_resources, planeEntityId);
	CreateRightWing(m_scene, m_resources, planeEntityId);
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