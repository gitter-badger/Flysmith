#pragma once
#include "PublicDef.h"


class KEBAB_API Component
{
public:
	void AttachToEntity(I32 entityId);
	I32 GetEntityId();

protected:
	Component();
	I32 entityId;
};