#pragma once
#include "PublicDef.h"


class KEBAB_API Component
{
public:
	enum Type
	{
		RENDER,
		PHYSICS,

		NUM_TYPES
	};

public:
	virtual ~Component();
	void AttachToEntity(I32 entityId);
	
	I32 GetEntityId() const;
	virtual Type GetType() const = 0;

protected:
	Component();
	I32 entityId;
};