#pragma once
#include "PublicDef.h"


// TODO: Temporary in renderer
// Will move it out, along with RenderComponent
class KEBAB12_API Component
{
public:
	void AttachToEntity(I32 entityId_)
	{
		entityId = entityId_;
	}

	I32 GetEntityId()
	{
		return entityId;
	}

protected:
	Component() : entityId(-1) {}
	I32 entityId;
};