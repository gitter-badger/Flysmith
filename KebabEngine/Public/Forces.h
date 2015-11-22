#pragma once
#include <vector>


class PhysicsComponent;

class IForceGenerator
{
public:
	virtual void UpdateForce(PhysicsComponent*, float dt) = 0;
};

class ForceRegistry
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