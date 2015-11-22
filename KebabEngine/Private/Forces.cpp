#include "PCH.h"
#include "Forces.h"


void ForceRegistry::AddForceGenerator(PhysicsComponent* physComp, IForceGenerator* forceGen)
{
	m_registrations.push_back({ physComp, forceGen });
}

void ForceRegistry::UpdateForces(float dt)
{
	for (auto it = m_registrations.begin(), end = m_registrations.end(); it != end; ++it)
	{
		it->forceGen->UpdateForce(it->object, dt);
	}
}