#pragma once
#include "Component.h"


// Describes a component stored somewhere else other than the Entity
struct ComponentProxy
{
	ComponentProxy(Component::Type, U32 componentIndex);

	Component::Type type;	
	// Index in the container holding components of same type
	U32 index;
};