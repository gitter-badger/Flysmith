#include "PCH.h"
#include "Component.h"


Component::Component() 
	: entityId(-1)
{
	//_CrtDbgBreak();
}

void Component::AttachToEntity(I32 entityId_)
{
	entityId = entityId_;
}

I32 Component::GetEntityId()
{
	return entityId;
}