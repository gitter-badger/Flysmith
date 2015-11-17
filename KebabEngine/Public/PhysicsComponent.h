#pragma once
#include "Component.h"
#include "../../KebabD3D12/Public/Vector.h"


class KEBAB_API PhysicsComponent : public Component
{
public:
	PhysicsComponent();

	Component::Type GetType() const override;

	// 0 inverse mass is equivalent to infinite mass 
	// while infinite inverse mass(mass = 0) is impossible.
	// Done to avoid division by 0 during integration(and also avoid the additional division each step).
	float inverseMass;
	
	Vector3 velocity;
	Vector3 acceleration;

	// Temp while testing physics inside Flysmith main. 
	// TODO: Replace with the aerodynamic drag model from TestsAero proj. 
	float damping;
};
