#pragma once
#include "Forces.h"
#include "PublicDef.h"
#include "../../KebabD3D12/Public/Vector.h"


class KEBAB_API GravityForceGenerator : public IForceGenerator
{
public:
	GravityForceGenerator(const Vector3& gravityAcc);
	void UpdateForce(PhysicsComponent*, float dt) override;

private:
	Vector3 m_gravityAcc;
};