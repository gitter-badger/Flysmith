#pragma once
#include "Component.h"
#include "../../KebabD3D12/Public/Vector.h"
#include "../../KebabD3D12/Public/Transform.h"


class KEBAB_API PhysicsComponent : public Component
{
public:
	PhysicsComponent();

	Component::Type GetType() const override;

	void SetMass(float);
	void SetInverseMass(float);
	void SetDamping(float);

	void SetVelocity(const Vector3&);
	void SetAcceleration(const Vector3&);

	void AddForce(const Vector3&);
	void ClearAccumulator();

	void Integrate(Transform*, float dt);

private:
	// Net force acting on object
	Vector3 m_forceAccum;

	// 0 inverse mass is equivalent to infinite mass 
	// while infinite inverse mass(mass = 0) is impossible
	// Done to avoid division by 0 during integration(and also avoid the additional division each step)
	float m_inverseMass;
	
	Vector3 m_velocity;
	Vector3 m_acceleration;

	// Temp while testing physics inside Flysmith main
	// TODO: Replace with the aerodynamic drag model from TestsAero proj
	float m_damping;
};