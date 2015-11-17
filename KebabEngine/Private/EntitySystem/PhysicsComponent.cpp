#include "PCH.h"
#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent()
	: inverseMass(0.0f)
	, damping(0.0f)
{
}

Component::Type PhysicsComponent::GetType() const
{
	return Component::Type::PHYSICS;
}
