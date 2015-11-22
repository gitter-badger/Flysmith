#pragma once
#include "Resources/ResourceRegistry.h"
#include "Scene.h"


namespace PlaneFactory
{
	inline void CreateFuselage(Scene& scene, ResourceRegistry& resources, EntityId planeId)
	{
		// Create dummy entity
		auto fuselageId = scene.CreateEntity();

		// Crate render component
		auto renderCompProxy = scene.CreateRenderComponent(resources.GetHandle("Fuselage"), resources.GetHandle("TestVS"), resources.GetHandle("TestPS"));
		scene.AttachComponent(fuselageId, renderCompProxy);

		// Create physics component
		auto physComponentProxy = scene.CreatePhysicsComponent();
		scene.AttachComponent(fuselageId, physComponentProxy);
		scene.physicsComponents[0].SetInverseMass(0.1f);
		scene.physicsComponents[0].SetVelocity({ 0.0f });
		scene.physicsComponents[0].SetAcceleration({ 0.0f });
		scene.physicsComponents[0].SetDamping(1.0f);

		// Attach the fuselage entity to the dummy master plane entity 
		scene.entities[planeId].AddChild(&scene.entities[fuselageId]);
	}

	inline void CreateLeftWing(Scene& scene, ResourceRegistry& resources, EntityId planeId)
	{
		// Create dummy entity
		auto wingId = scene.CreateEntity();

		// Set initial transform(relative to the master plane dummy)
		scene.entities[wingId].GetTransform()->MirrorAlongX();
		scene.entities[wingId].GetTransform()->RotateY(XM_PIDIV2);

		// Crate render component
		auto renderCompProxy = scene.CreateRenderComponent(resources.GetHandle("Wing"), resources.GetHandle("TestVS"), resources.GetHandle("TestPS"));
		scene.AttachComponent(wingId, renderCompProxy);

		// Create physics component

		// Attach the fuselage entity to the dummy master plane entity 
		scene.entities[planeId].AddChild(&scene.entities[wingId]);
	}

	inline void CreateRightWing(Scene& scene, ResourceRegistry& resources, EntityId planeId)
	{
		// Create dummy entity
		auto wingId = scene.CreateEntity();

		// Set initial transform(relative to the master plane dummy)
		scene.entities[wingId].GetTransform()->RotateY(-XM_PIDIV2);

		// Crate render component
		auto renderCompProxy = scene.CreateRenderComponent(resources.GetHandle("Wing"), resources.GetHandle("TestVS"), resources.GetHandle("TestPS"));
		scene.AttachComponent(wingId, renderCompProxy);

		// Create physics component

		// Attach the fuselage entity to the dummy master plane entity 
		scene.entities[planeId].AddChild(&scene.entities[wingId]);
	}

	inline void Create(ResourceRegistry& resources, Scene& scene)
	{
		auto planeEntityId = scene.CreateEntity();
		CreateFuselage(scene, resources, planeEntityId);
		CreateLeftWing(scene, resources, planeEntityId);
		CreateRightWing(scene, resources, planeEntityId);
	}
}