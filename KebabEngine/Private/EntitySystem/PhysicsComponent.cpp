#include "PCH.h"
#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent()
	: m_inverseMass(0.0f)
	, m_damping(0.0f)
{
}

Component::Type PhysicsComponent::GetType() const
{
	return Component::Type::PHYSICS;
}

void PhysicsComponent::SetMass(float mass)
{
	assert(mass > 0.0f);
	m_inverseMass = 1.0f / mass;
}

void PhysicsComponent::SetInverseMass(float inverseMass)
{
	// The inverse mass may be 0, equivalent to infinite mass
	assert(inverseMass >= 0.0f);
	inverseMass = inverseMass;
}

void PhysicsComponent::SetDamping(float damping)
{
	m_damping = damping;
}

void PhysicsComponent::SetVelocity(const Vector3& velocity)
{
	m_velocity = velocity;
}

void PhysicsComponent::SetAcceleration(const Vector3& acceleration)
{
	m_acceleration = acceleration;
}

void PhysicsComponent::AddForce(const Vector3& force)
{
	m_forceAccum += force;
}

void PhysicsComponent::ClearAccumulator()
{
	m_forceAccum = { 0 };
}

void PhysicsComponent::Integrate(Transform* xform, float dt)
{
	// Ignore things with infinite mass
	if (m_inverseMass <= 0.0f) return;

	// Update position
	auto vel = m_velocity.GetXMVec();
	auto pos = xform->GetPositionXM();
	xform->SetPosition(pos + vel * dt);

	// Acceleration from force
	// a = inverseMass * forceAccum
	Vector3 resultingAcc = m_acceleration;
	resultingAcc += m_forceAccum * m_inverseMass;

	// Update velocity from acceleration
	m_velocity += resultingAcc * dt;

	// Impose drag
	m_velocity *= powf(dt, m_damping);

	// Clear forces
	m_forceAccum = { 0 };
}