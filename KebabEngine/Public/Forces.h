#pragma once
#include <vector>
#include "PublicDef.h"


class PhysicsComponent;

class KEBAB_API IForceGenerator
{
public:
	virtual void UpdateForce(PhysicsComponent*, float dt) = 0;
};

class KEBAB_API ForceRegistry
{
public:
	void AddForceGenerator(PhysicsComponent*, IForceGenerator*);
	void UpdateForces(float dt);

private:
	struct ForceRegistration
	{
		PhysicsComponent* object;
		IForceGenerator* forceGen;
	};

	std::vector<ForceRegistration> m_registrations;
};