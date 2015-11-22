#include "PCH.h"
#include "GravityForceGenerator.h"
#include "PhysicsComponent.h"


GravityForceGenerator::GravityForceGenerator(const Vector3& gravityAcc)
	: m_gravityAcc(gravityAcc)
{
}

void GravityForceGenerator::UpdateForce(PhysicsComponent* pPhysComp, float dt)
{
	if (pPhysComp->GetMass()) return;

	pPhysComp->AddForce(m_gravityAcc * pPhysComp->GetMass());
}