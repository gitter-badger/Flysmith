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

void PhysicsComponent::SetMass(float newMass)
{
	assert(newMass > 0.0f);
	inverseMass = 1.0f / newMass;
}

void PhysicsComponent::SetInverseMass(float newInverseMass)
{
	// The inverse mass may be 0, equivalent to infinite mass
	assert(newInverseMass >= 0.0f);
	inverseMass = newInverseMass;
}

void PhysicsComponent::SetDamping(float newDamping)
{
	damping = newDamping;
}

void PhysicsComponent::SetVelocity(const Vector3& newVelocity)
{
	velocity = newVelocity;
}

void PhysicsComponent::SetAcceleration(const Vector3& newAcceleration)
{
	acceleration = newAcceleration;
}

void PhysicsComponent::AddForce(const Vector3& force)
{
	forceAccum += force;
}

void PhysicsComponent::ClearAccumulator()
{
	forceAccum = { 0 };
}

void PhysicsComponent::Integrate(Transform* xform, float dt)
{
	// Ignore things with infinite mass
	if (inverseMass <= 0.0f) return;

	// Update position
	auto vel = velocity.GetXMVec();
	auto pos = xform->GetPositionXM();
	xform->SetPosition(pos + vel * dt);

	// Acceleration from force
	// a = inverseMass * forceAccum
	Vector3 resultingAcc = acceleration;
	resultingAcc += forceAccum * inverseMass;

	// Update velocity from acceleration
	velocity += resultingAcc * dt;

	// Impose drag
	velocity *= powf(dt, damping);

	// Clear forces
	forceAccum = { 0 };
}